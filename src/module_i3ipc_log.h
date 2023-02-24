#ifndef _I3_MANAGER_MODULE_I3IPC_LOG_H
#define _I3_MANAGER_MODULE_I3IPC_LOG_H

// ======================================================================

#include <fmt/format.h>
#include <logger.h>

// ----------------------------------------------------------------------

#include "module.h"

// ======================================================================

class Module_i3ipc_log : public Module {
public:
	Module_i3ipc_log( i3ipc::connection& c, logger::Depth depth );

public:
	void handle_output_event() override;
	void handle_workspace_event( const i3ipc::workspace_event_t& event ) override;
	void handle_binding_event( const i3ipc::binding_t& event ) override;
	void handle_mode_event( const i3ipc::mode_t& event ) override;
	void handle_window_event( const i3ipc::window_event_t& event ) override;

private:
	logger::Logger m_log;
};

// ======================================================================

#endif
