// ======================================================================

#include "module_multi_monitor.h"

// ----------------------------------------------------------------------

#include <consoler.h>
#include <fmt/format.h>
#include <i3ipc++/ipc.hpp>

// ======================================================================


Module_multi_monitor::Module_multi_monitor( i3ipc::connection& c, logger::Depth depth ) :
	Module( c ), m_log( "OUTPUT" ) {
	logger::Settings& settings = m_log.get_settings();
	settings.file_enabled	   = false;
	settings.visible_depth	   = depth;


	// Set the primary output to number 1
	std::vector< std::shared_ptr< i3ipc::output_t > > outputs = m_i3.get_outputs();
	for( auto it = outputs.begin(); it != outputs.end(); ++it ) {
		if( ( *it )->primary ) {
			m_outputs[1] = ( *it )->name;
			outputs.erase( it );
		}
	}

	// Set the rest in any order
	for( std::shared_ptr< i3ipc::output_t > output : outputs )
		m_outputs[m_outputs.size() + 1] = output->name;

	init_workspaces();
}

// ----------------------------------------------------------------------

void Module_multi_monitor::handle_binding_event( const i3ipc::binding_t& event ) {
	std::vector< std::string > commands = logger::split_string( event.command, " " );

	if( *commands.begin() != "i3m" || commands.size() < 2u )
		return;

	commands.erase( commands.begin() );
	if( commands.empty() )
		return;

	{
		std::string str;
		for( std::string command : commands )
			str += command + ' ';

		m_log.log( fmt::format( "Processing command {}", str ), logger::Depth::INFO );
	}

	if( *commands.begin() == "workspace" ) {
		commands.erase( commands.begin() );
		command_focus_workspace( commands );
		return;
	}

	if( *commands.begin() == "output" ) {
		commands.erase( commands.begin() );
		command_focus_output( commands );
		return;
	}

	if( *commands.begin() == "set" ) {
		commands.erase( commands.begin() );
		if( commands.empty() )
			return error_argument_expected( "set", "output" );

		if( *commands.begin() == "output" ) {
			commands.erase( commands.begin() );
			command_set_output( commands );
			return;
		}
	}

	if( *commands.begin() == "move" ) {
		commands.erase( commands.begin() );
		command_move_workspace( commands );
	}
}

// ----------------------------------------------------------------------

void Module_multi_monitor::handle_workspace_event( const i3ipc::workspace_event_t& event ) {
	if( event.current )
		m_current = event.current->num;
}

// ======================================================================

void Module_multi_monitor::focus_output( size_t output ) {
	const auto it = m_outputs.find( output );
	if( it == m_outputs.end() )
		return error_invalid_argument(
			"output", { std::to_string( output ) }, 0u, "Output not found" );

	send_command( fmt::format( "focus output {}", it->second ) );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::focus_output( std::string output ) {
	const auto it = std::find_if( m_outputs.begin(),
								  m_outputs.end(),
								  [output]( const std::pair< size_t, std::string >& rh ) {
									  return rh.second == output;
								  } );

	if( it == m_outputs.end() )
		return error_invalid_argument( "output", { output }, 0u, "Output not found" );

	send_command( fmt::format( "focus workspace number {}", output ) );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::focus_workspace( size_t workspace ) {
	const size_t output_num	   = m_current / 10u;
	const size_t workspace_num = output_num * 10 + workspace;

	send_command( fmt::format( "focus workspace number {}", workspace_num ) );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::focus_workspace_all( size_t workspace ) {
	const size_t output_num = m_current / 10u;

	for( std::pair< size_t, std::string > output : m_outputs ) {
		const size_t workspace_num = output.first * 10u + workspace;
		send_command( fmt::format( "focus workspace number {}", workspace_num ) );
	}

	const size_t workspace_num = output_num * 10u + workspace;
	send_command( fmt::format( "focus workspace number {}", workspace_num ) );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::move_workspace( size_t workspace ) {
	const size_t output_num	   = m_current / 10u;
	const size_t workspace_num = output_num * 10u + workspace;

	send_command( fmt::format( "move container to workspace number {}", workspace_num ) );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::set_output( std::string output, size_t idx ) {
	const auto output_source
		= std::find_if( m_outputs.begin(),
						m_outputs.end(),
						[output]( const std::pair< size_t, std::string >& rh ) {
							return rh.second == output;
						} );

	if( output_source == m_outputs.end() )
		return error_invalid_argument(
			"set output", { output, std::to_string( idx ) }, 1u, "Output not found" );

	const auto output_target = m_outputs.find( idx );
	if( output_target == m_outputs.end() ) {
		m_outputs[idx] = output;
		m_outputs.erase( output_source );
	}
	// Swap outputs mappings if the target index already has an output assigned
	else {
		std::string& output_source_str = output_source->second;
		std::string& output_target_str = output_target->second;

		std::swap( output_source_str, output_target_str );
	}

	init_workspaces();
}

// ======================================================================

void Module_multi_monitor::command_focus_output( std::vector< std::string > args ) {
	if( args.empty() )
		return error_argument_expected( "output", "[number]" );

	try {
		return focus_output( std::stoul( *args.begin() ) );
	}
	catch( ... ) {
		return focus_output( *args.begin() );
	}
}

// ----------------------------------------------------------------------

void Module_multi_monitor::command_focus_workspace( std::vector< std::string > args ) {
	if( args.empty() )
		return error_argument_expected( "workspace", "[number]/all" );

	if( *args.begin() == "all" ) {
		args.erase( args.begin() );

		if( args.empty() )
			return error_argument_expected( "workspace all", "[number]" );

		try {
			return focus_workspace_all( std::stoul( *args.begin() ) );
		}
		catch( ... ) {
			return error_invalid_argument( "workspace all", args, 0u, "Expected a number" );
		}
	}

	try {
		return focus_workspace( std::stoul( *args.begin() ) );
	}
	catch( ... ) {
		return error_invalid_argument( "workspace", args, 0u, "Expected a number" );
	}
}

// ----------------------------------------------------------------------

void Module_multi_monitor::command_move_workspace( std::vector< std::string > args ) {
	if( args.empty() )
		return error_argument_expected( "move", "number" );

	try {
		return move_workspace( std::stoul( *args.begin() ) );
	}
	catch( ... ) {
		return error_invalid_argument( "move", args, 0u, "Expected a number" );
	}
}

// ----------------------------------------------------------------------

void Module_multi_monitor::command_set_output( std::vector< std::string > args ) {
	if( args.empty() )
		return error_argument_expected( "set output", "[output name]" );

	const std::string output = *args.begin();
	args.erase( args.begin() );
	if( args.empty() )
		return error_argument_expected( "set output " + output, "number" );

	try {
		return set_output( output, std::stoul( *args.begin() ) );
	}
	catch( ... ) {
		return error_invalid_argument( "set output " + output, args, 0u, "Expected a number" );
	}
}

// ======================================================================

void Module_multi_monitor::init_workspaces() {
	for( std::pair< size_t, std::string > output : m_outputs ) {
		for( int i = 0; i <= 9; ++i ) {
			const int workspace		  = ( output.first * 10 ) + i;
			const std::string command = fmt::format(
				"workspace number {} output {}", workspace, output.second );
			m_i3.send_command( command );
			m_log.log( command, logger::Depth::DEBUG );
		}
	}

	focus_workspace_all( 1u );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::send_command( std::string command ) {
	m_log.log( command, logger::Depth::DEBUG );
	m_i3.send_command( command );
}

// ======================================================================

void Module_multi_monitor::error_invalid_argument( std::string command,
												   const std::vector< std::string >& args,
												   size_t problem,
												   std::string reason ) const {
	std::string str = command;
	for( size_t i = 0u; i < args.size(); ++i ) {
		str += ' ';

		if( i == problem )
			str += console::foreground(
				console::underline( args.at( i ), console::Underline_Mode::SINGLE ),
				console::Color_16::BRIGHT_RED );
		else
			str += args.at( i );
	}

	m_log.log( fmt::format( "Invalid argument: {}\nReason: {}", command, reason ),
			   logger::Depth::ERROR );
}

// ----------------------------------------------------------------------

void Module_multi_monitor::error_argument_expected( std::string command, std::string type ) {
	m_log.log( fmt::format( "Command {} expects an argument of type {}", command, type ),
			   logger::Depth::ERROR );
}

// ======================================================================
