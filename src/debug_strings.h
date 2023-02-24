#ifndef _I3_MANAGER_WORKSPACE_H
#define _I3_MANAGER_WORKSPACE_H

// ======================================================================

#include <i3ipc++/ipc.hpp>
#include <string.h>

// ======================================================================

std::string to_string( i3ipc::EventType in );
std::string to_string( i3ipc::WorkspaceEventType in );
std::string to_string( i3ipc::WindowEventType in );
std::string to_string( i3ipc::BorderStyle in );
std::string to_string( i3ipc::ContainerLayout in );
std::string to_string( i3ipc::InputType in );
std::string to_string( i3ipc::BarMode in );
std::string to_string( i3ipc::Position in );

std::string to_string( const i3ipc::workspace_t& workspace );
std::string to_string( const i3ipc::workspace_event_t& event );

std::string to_string( const i3ipc::container_t& container );
std::string to_string( const i3ipc::window_event_t& event );
std::string to_string( const i3ipc::window_properties_t& properties );

std::string to_string( const i3ipc::binding_t& binding );
std::string to_string( const i3ipc::mode_t& mode );
std::string to_string( const i3ipc::rect_t& rect );
std::string to_string( const i3ipc::output_t& output );

// ======================================================================

#endif
