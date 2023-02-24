// ======================================================================

#include "debug_strings.h"

// ----------------------------------------------------------------------

#include <fmt/format.h>
#include <logger.h>
#include <unordered_map>

// ======================================================================

std::unordered_map< i3ipc::EventType, std::string > _event_type_string_map{
	{ i3ipc::EventType::ET_WORKSPACE, "Workspace" },
	{ i3ipc::EventType::ET_OUTPUT, "Output" },
	{ i3ipc::EventType::ET_MODE, "Mode" },
	{ i3ipc::EventType::ET_WINDOW, "Window" },
	{ i3ipc::EventType::ET_BINDING, "Binding" },
};

std::unordered_map< i3ipc::WorkspaceEventType, std::string > _workspace_event_type_string_map{
	{ i3ipc::WorkspaceEventType::FOCUS, "Focus" },
	{ i3ipc::WorkspaceEventType::INIT, "Init" },
	{ i3ipc::WorkspaceEventType::EMPTY, "Empty" },
	{ i3ipc::WorkspaceEventType::URGENT, "Urgent" },
	{ i3ipc::WorkspaceEventType::RENAME, "Rename" },
	{ i3ipc::WorkspaceEventType::RELOAD, "Reload" },
	{ i3ipc::WorkspaceEventType::RESTORED, "Restored" },
};

std::unordered_map< i3ipc::WindowEventType, std::string > _window_event_type_string_map{
	{ i3ipc::WindowEventType::NEW, "New" },
	{ i3ipc::WindowEventType::CLOSE, "Close" },
	{ i3ipc::WindowEventType::FOCUS, "Focus" },
	{ i3ipc::WindowEventType::TITLE, "Title" },
	{ i3ipc::WindowEventType::FULLSCREEN_MODE, "Fullscreen Mode" },
	{ i3ipc::WindowEventType::MOVE, "Move" },
	{ i3ipc::WindowEventType::FLOATING, "Floating" },
	{ i3ipc::WindowEventType::URGENT, "Urgent" },
};

std::unordered_map< i3ipc::BorderStyle, std::string > _border_style_string_map{
	{ i3ipc::BorderStyle::UNKNOWN, "Unknown" },
	{ i3ipc::BorderStyle::NONE, "None" },
	{ i3ipc::BorderStyle::NORMAL, "Normal" },
	{ i3ipc::BorderStyle::PIXEL, "Pixel" },
	{ i3ipc::BorderStyle::ONE_PIXEL, "One Pixel" },
};

std::unordered_map< i3ipc::ContainerLayout, std::string > _container_layout_string_map{
	{ i3ipc::ContainerLayout::UNKNOWN, "Unknown " },
	{ i3ipc::ContainerLayout::SPLIT_H, "Split H" },
	{ i3ipc::ContainerLayout::SPLIT_V, "Split V" },
	{ i3ipc::ContainerLayout::STACKED, "Stacked" },
	{ i3ipc::ContainerLayout::TABBED, "Tabbed" },
	{ i3ipc::ContainerLayout::DOCKAREA, "Dock Area" },
	{ i3ipc::ContainerLayout::OUTPUT, "Output" },
};

std::unordered_map< i3ipc::InputType, std::string > _input_type_string_map{
	{ i3ipc::InputType::UNKNOWN, "Unknown" },
	{ i3ipc::InputType::KEYBOARD, "Keyboard" },
	{ i3ipc::InputType::MOUSE, "Mouse" },
};

std::unordered_map< i3ipc::BarMode, std::string > _bar_mode_string_map{
	{ i3ipc::BarMode::UNKNOWN, "Unknown" },
	{ i3ipc::BarMode::DOCK, "Dock" },
	{ i3ipc::BarMode::HIDE, "Hide" },
};

std::unordered_map< i3ipc::Position, std::string > _position_string_map{
	{ i3ipc::Position::UNKNOWN, "Unknown" },
	{ i3ipc::Position::TOP, "Top" },
	{ i3ipc::Position::BOTTOM, "Bottom" },
};

// ======================================================================

std::string to_string( i3ipc::EventType in ) {
	return _event_type_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::WorkspaceEventType in ) {
	return _workspace_event_type_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::WindowEventType in ) {
	return _window_event_type_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::BorderStyle in ) {
	return _border_style_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::ContainerLayout in ) {
	return _container_layout_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::InputType in ) {
	return _input_type_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::BarMode in ) {
	return _bar_mode_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( i3ipc::Position in ) {
	return _position_string_map.at( in );
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::workspace_t& workspace ) {
	std::string out;

	out += fmt::format( "num: {}\n", workspace.num );
	out += fmt::format( "name: {}\n", workspace.name );
	out += fmt::format( "visible: {}\n", workspace.visible );
	out += fmt::format( "focused: {}\n", workspace.focused );
	out += fmt::format( "urgent: {}\n", workspace.urgent );
	out += fmt::format( "rect: {}\n", to_string( workspace.rect ) );
	out += fmt::format( "output: {}\n", workspace.output );

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::workspace_event_t& event ) {
	std::string out;

	out += fmt::format( "Type: {}\n", to_string( event.type ) );

	if( event.current )
		out += fmt::format( "Current:\n{}\n",
							logger::indent_string( to_string( *event.current ), 4u ) );
	else
		out += "Current: None\n";

	if( event.old )
		out += fmt::format(
			"Old: \n{}\n", logger::indent_string( to_string( *event.old ), 4u ) );
	else
		out += "Old: None\n";

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::container_t& container ) {
	std::string out;

	out += fmt::format( "id: {}\n", container.id );
	out += fmt::format( "xwindow_id: {}\n", container.xwindow_id );
	out += fmt::format( "name: {}\n", container.name );
	out += fmt::format( "type: {}\n", container.type );
	out += fmt::format( "border: {}\n", to_string( container.border ) );
	out += fmt::format( "border_raw: {}\n", container.border_raw );
	out += fmt::format( "current_border_width: {}\n", container.current_border_width );
	out += fmt::format( "layout: {}\n", to_string( container.layout ) );
	out += fmt::format( "layout_raw: {}\n", container.layout_raw );
	out += fmt::format( "percent: {}\n", container.percent );
	out += fmt::format( "rect: {}\n", to_string( container.rect ) );
	out += fmt::format( "window_rect: {}\n", to_string( container.window_rect ) );
	out += fmt::format( "deco_rect: {}\n", to_string( container.deco_rect ) );
	out += fmt::format( "geometry: {}\n", to_string( container.geometry ) );
	out += fmt::format( "urgent: {}\n", container.urgent );
	out += fmt::format( "focused: {}\n", container.focused );

	if( container.workspace.has_value() )
		out += fmt::format( "workspace: {}\n", container.workspace.value() );

	out += fmt::format(
		"window_properties:\n{}\n",
		logger::indent_string( to_string( container.window_properties ), 4u ) );

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::window_event_t& event ) {
	std::string out;

	out += fmt::format( "Type: {}\n", to_string( event.type ) );
	out += fmt::format( "Container:\n{}\n",
						logger::indent_string( to_string( *event.container ), 4u ) );

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::window_properties_t& properties ) {
	std::string out;

	out += fmt::format( "xclass: {}\n", properties.xclass );
	out += fmt::format( "instance: {}\n", properties.instance );
	out += fmt::format( "window_role: {}\n", properties.window_role );
	out += fmt::format( "title: {}\n", properties.title );
	out += fmt::format( "transient_for: {}\n", properties.transient_for );

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::binding_t& binding ) {
	std::string out;

	out += fmt::format( "command: {}\n", binding.command );

	if( !binding.event_state_mask.empty() ) {
		out += fmt::format( "event_state_mask: " );
		for( std::string str : binding.event_state_mask )
			out += str + ", ";
		out.erase( out.length() - 2u, 2u );
		out += '\n';
	}

	out += fmt::format( "input_code: {}\n", binding.input_code );
	out += fmt::format( "symbol: {}\n", binding.symbol );
	out += fmt::format( "input_type: {}\n", to_string( binding.input_type ) );

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::mode_t& mode ) {
	std::string out;

	out += fmt::format( "change: {}\n", mode.change );
	out += fmt::format( "pango_markup: {}\n", mode.pango_markup );

	return out;
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::rect_t& rect ) {
	return fmt::format( "{}, {}, {}, {}", rect.x, rect.y, rect.width, rect.height );
}

// ----------------------------------------------------------------------

std::string to_string( const i3ipc::output_t& output ) {
	std::string out;

	out += fmt::format( "name: {}\n", output.name );
	out += fmt::format( "active: {}\n", output.active );
	out += fmt::format( "primary: {}\n", output.primary );
	out += fmt::format( "current_workspace: {}\n", output.current_workspace );
	out += fmt::format( "rect: {}\n", to_string( output.rect ) );

	return out;
}

// ======================================================================
