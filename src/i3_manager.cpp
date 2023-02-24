// ======================================================================

#include <fmt/format.h>
#include <i3ipc++/ipc.hpp>
#include <logger.h>

// ----------------------------------------------------------------------

#include "module.h"
#include "module_i3ipc_log.h"

// ======================================================================

logger::Logger _core_log;

std::vector< Module* > _modules;

// ----------------------------------------------------------------------

void handle_output_event();
void handle_workspace_event( const i3ipc::workspace_event_t& event );
void handle_binding_event( const i3ipc::binding_t& event );
void handle_mode_event( const i3ipc::mode_t& event );
void handle_window_event( const i3ipc::window_event_t& event );

// ======================================================================

int main( int argc, char* argv[] ) {
	{
		logger::Settings& settings = _core_log.get_settings();
		settings.file_enabled	   = false;

#ifdef _DEBUG
		settings.visible_depth = logger::Depth::TRACE;
#endif
	}

	i3ipc::connection i3;

#ifdef _DEBUG
	_modules.push_back( new Module_i3ipc_log( i3, logger::Depth::TRACE ) );
#endif

	_core_log.log_separator( '=' );
	_core_log.log_separator( "I3 MANAGER" );
	_core_log.log_separator( '=' );

	_core_log.log( "Connecting to i3 service" );

	// Connection
	i3.connect_event_socket();

	i3.subscribe( i3ipc::ET_OUTPUT | i3ipc::ET_WORKSPACE | i3ipc::ET_BINDING
				  | i3ipc::ET_MODE | i3ipc::ET_WINDOW );

	// Binds
	_core_log.log( "Binding i3 events" );

	_core_log.log( "output events...", logger::Depth::INFO, 4u );
	sigc::signal< void > i3ipc_output_signal;
	i3ipc_output_signal.connect( sigc::ptr_fun( &handle_output_event ) );
	i3.signal_output_event = i3ipc_output_signal;

	_core_log.log( "workspace events...", logger::Depth::INFO, 4u );
	sigc::signal< void, const i3ipc::workspace_event_t& > i3ipc_workspace_signal;
	i3ipc_workspace_signal.connect( sigc::ptr_fun( &handle_workspace_event ) );
	i3.signal_workspace_event = i3ipc_workspace_signal;

	_core_log.log( "binding events...", logger::Depth::INFO, 4u );
	sigc::signal< void, const i3ipc::binding_t& > i3ipc_binding_signal;
	i3ipc_binding_signal.connect( sigc::ptr_fun( &handle_binding_event ) );
	i3.signal_binding_event = i3ipc_binding_signal;

	_core_log.log( "mode events...", logger::Depth::INFO, 4u );
	sigc::signal< void, const i3ipc::mode_t& > i3ipc_mode_signal;
	i3ipc_mode_signal.connect( sigc::ptr_fun( &handle_mode_event ) );
	i3.signal_mode_event = i3ipc_mode_signal;

	_core_log.log( "window events...", logger::Depth::INFO, 4u );
	sigc::signal< void, const i3ipc::window_event_t& > i3ipc_window_signal;
	i3ipc_window_signal.connect( sigc::ptr_fun( &handle_window_event ) );
	i3.signal_window_event = i3ipc_window_signal;


	_core_log.log( "Starting daemon" );
	_core_log.log_separator( '=' );

	while( true ) {
		i3.handle_event();
	}

	return 0;
}

// ======================================================================

void handle_output_event() {
	for( Module* module : _modules )
		module->handle_output_event();
}

// ----------------------------------------------------------------------

void handle_workspace_event( const i3ipc::workspace_event_t& event ) {
	for( Module* module : _modules )
		module->handle_workspace_event( event );
}

// ----------------------------------------------------------------------

void handle_binding_event( const i3ipc::binding_t& event ) {
	for( Module* module : _modules )
		module->handle_binding_event( event );
}

// ----------------------------------------------------------------------

void handle_mode_event( const i3ipc::mode_t& event ) {
	for( Module* module : _modules )
		module->handle_mode_event( event );
}

// ----------------------------------------------------------------------

void handle_window_event( const i3ipc::window_event_t& event ) {
	for( Module* module : _modules )
		module->handle_window_event( event );
}

// ======================================================================
