// ======================================================================

#include "module_i3ipc_log.h"

// ----------------------------------------------------------------------

#include <fmt/format.h>

// ----------------------------------------------------------------------

#include "debug_strings.h"

// ======================================================================

Module_i3ipc_log::Module_i3ipc_log( i3ipc::connection& c, logger::Depth depth ) :
	Module( c ), m_log( "I3IPC" ) {
	logger::Settings& settings = m_log.get_settings();
	settings.file_enabled	   = false;
	settings.visible_depth	   = depth;
}

// ----------------------------------------------------------------------

void Module_i3ipc_log::handle_output_event() {
	//
}

// ----------------------------------------------------------------------

void Module_i3ipc_log::handle_workspace_event( const i3ipc::workspace_event_t& event ) {
	m_log.log( fmt::format( "Workspace Event: {}", to_string( event.type ) ),
			   logger::Depth::DEBUG );
	m_log.log( logger::indent_string( to_string( event ), 4u ), logger::Depth::DEBUG );
	m_log.log_separator( '=', logger::Depth::TRACE );
}

// ----------------------------------------------------------------------

void Module_i3ipc_log::handle_binding_event( const i3ipc::binding_t& event ) {
	m_log.log( fmt::format( "Binding Event: {}", to_string( event.input_type ) ),
			   logger::Depth::DEBUG );

	std::string str = to_string( event );
	str				= logger::indent_string( str, 4u );
	m_log.log( str, logger::Depth::TRACE );
	m_log.log_separator( '=', logger::Depth::TRACE );
}

// ----------------------------------------------------------------------

void Module_i3ipc_log::handle_mode_event( const i3ipc::mode_t& event ) {
	m_log.log( fmt::format( "Mode Event: {}", event.change ), logger::Depth::DEBUG );
}

// ----------------------------------------------------------------------

void Module_i3ipc_log::handle_window_event( const i3ipc::window_event_t& event ) {
	m_log.log( fmt::format( "Window Event: {}", to_string( event.type ) ),
			   logger::Depth::DEBUG );
	m_log.log( logger::indent_string( to_string( event ), 4u ), logger::Depth::TRACE );
	m_log.log_separator( '=', logger::Depth::TRACE );
}

// ======================================================================
