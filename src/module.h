#ifndef _I3_MANAGER_MODULE_H
#define _I3_MANAGER_MODULE_H

// ======================================================================

#include <i3ipc++/ipc.hpp>

// ======================================================================

class Module {
public:
	Module( i3ipc::connection& c ) : m_i3{ c } {}

public:
	virtual void handle_output_event() {}
	virtual void handle_workspace_event( const i3ipc::workspace_event_t& event ) {}
	virtual void handle_binding_event( const i3ipc::binding_t& event ) {}
	virtual void handle_mode_event( const i3ipc::mode_t& event ) {}
	virtual void handle_window_event( const i3ipc::window_event_t& ) {}

protected:
	i3ipc::connection& m_i3;
};

// ======================================================================

#endif
