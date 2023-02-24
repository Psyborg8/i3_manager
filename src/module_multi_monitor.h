#ifndef _I3_MANAGER_MODULE_MULTI_MONITOR_H
#define _I3_MANAGER_MODULE_MULTI_MONITOR_H

// ======================================================================

#include <logger.h>

// ----------------------------------------------------------------------

#include "module.h"

// ======================================================================

class Module_multi_monitor : public Module {
public:
	Module_multi_monitor( i3ipc::connection& c, logger::Depth depth );

public:
	void handle_binding_event( const i3ipc::binding_t& event ) override;
	void handle_workspace_event( const i3ipc::workspace_event_t& event ) override;

private:
	void focus_output( int output );
	void focus_workspace( int workspace );

private:
	void error_invalid_argument( const std::vector< std::string >& args,
								 size_t problem,
								 std::string reason = "" ) const;

private:
	logger::Logger m_log;
	std::shared_ptr< i3ipc::output_t > m_current;
};

// ======================================================================

#endif
