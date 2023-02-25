#ifndef _I3_MANAGER_MODULE_MULTI_MONITOR_H
#define _I3_MANAGER_MODULE_MULTI_MONITOR_H

// ======================================================================

#include <logger.h>
#include <unordered_map>
#include <vector>

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
	void focus_output( size_t output );
	void focus_output( std::string output );
	void focus_workspace( size_t workspace );
	void focus_workspace_all( size_t workspace );
	void move_workspace( size_t workspace );
	void set_output( std::string output, size_t idx );

private:
	void command_focus_output( std::vector< std::string > args );
	void command_focus_workspace( std::vector< std::string > args );
	void command_move_workspace( std::vector< std::string > args );
	void command_set_output( std::vector< std::string > args );

private:
	void init_workspaces();
	void send_command( std::string command );

private:
	void error_invalid_argument( std::string command,
								 const std::vector< std::string >& args,
								 size_t problem		= std::string::npos,
								 std::string reason = "" ) const;
	void error_argument_expected( std::string command, std::string type );

private:
	logger::Logger m_log;
	size_t m_current;
	std::unordered_map< size_t, std::string > m_outputs;
};

// ======================================================================

#endif
