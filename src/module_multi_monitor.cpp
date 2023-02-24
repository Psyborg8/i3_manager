// ======================================================================

#include "module_multi_monitor.h"

// ----------------------------------------------------------------------

#include <fmt/format.h>

// ======================================================================


Module_multi_monitor::Module_multi_monitor( i3ipc::connection& c, logger::Depth depth ) :
	Module( c ), m_log( "OUTPUT" ) {
	logger::Settings& settings = m_log.get_settings();
	settings.file_enabled	   = false;
	settings.visible_depth	   = depth;


	std::vector< std::shared_ptr< i3ipc::output_t > > outputs = m_i3.get_outputs();
	m_log.log( "Outputs:" );

	for( size_t i = 0; i < outputs.size(); ++i )
		m_log.log( fmt::format( "{}: {}", i, outputs.at( i )->name ) );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::handle_binding_event( const i3ipc::binding_t& event ) {
	std::vector< std::string > commands = logger::split_string( event.command, " " );

	if( commands.at( 0 ) != "i3m" || commands.size() < 2u )
		return;

	if( commands.at( 1 ) == "output" && commands.size() == 3u ) {
		int output = -1;
		try {
			output = std::stoi( commands.at( 2u ) );
			focus_output( output );
		}
		catch( ... ) {
			error_invalid_argument( commands, 2u, "Expected a number" );
		}

		return;
	}

	if( commands.at( 1 ) == "workspace" && commands.size() == 3u ) {
		int workspace = -1;
		try {
			workspace = std::stoi( commands.at( 2u ) );
			focus_workspace( workspace );
		}
		catch( ... ) {
			error_invalid_argument( commands, 2u, "Expected a number" );
		}

		return;
	}
}

// ----------------------------------------------------------------------

void Module_multi_monitor::handle_workspace_event( const i3ipc::workspace_event_t& event ) {
	if( event.current ) {
		for( std::shared_ptr< i3ipc::output_t > output : m_i3.get_outputs() ) {
			if( output->name == event.current->output ) {
				m_current = output;
				break;
			}
		}
	}
}

// ----------------------------------------------------------------------

void Module_multi_monitor::error_invalid_argument( const std::vector< std::string >& args,
												   size_t problem,
												   std::string reason ) const {
	std::string command;
	for( size_t i = 0u; i < args.size(); ++i ) {
		command += args.at( i );
	}
}

// ======================================================================
