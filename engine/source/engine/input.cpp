#include <engine/input.hpp>
#include <engine/logger.hpp>
#include <engine/window.hpp>
#include <engine/dev_console.hpp>

#include <GLFW/glfw3.h>

namespace con
{
void Input::initialize( Window& window )
{
	bindings.initialize( CON_MAX_INPUT_BINDINGS );
	// Array doesn't construct objects for us, so we're doing it here.
	for ( s32 i = 0; i < bindings.size(); ++i ) {
		bindings[i] = Key_Binding{};
	}

	window_handle = window.get_internal_handle();

	glfwSetKeyCallback(         window_handle, Input::glfw_keyboard_key_callback );
	glfwSetMouseButtonCallback( window_handle, Input::glfw_mouse_key_callback );
	glfwSetScrollCallback(      window_handle, Input::glfw_mouse_scroll_callback );
}

void Input::shutdown()
{
	bindings.shutdown();
}

void Input::add_binding( Hashed_CString name, Key_ID key )
{
	con_assert( name.hash != 0 );
	con_assert( key != -1 );

	bool adding_succeed = false;

	for ( s32 i = 0; i < bindings.size() && !adding_succeed; ++i ) {
		auto& binding = bindings[i];

		if ( binding.name_hash == 0 && binding.key_id == -1 ) {
			binding.name_hash = name.hash;
			binding.key_id = key;
			adding_succeed = true;
		}
	}

	con_assert( adding_succeed );
}

void Input::change_binding( Hashed_CString name, Key_ID key )
{
	con_assert( name.hash != 0 );
	con_assert( key != -1 );

	bool changing_succeed = false;

	for ( s32 i = 0; i < bindings.size() && !changing_succeed; ++i ) {
		auto& binding = bindings[i];

		if ( binding.name_hash == name.hash ) {
			binding.key_id = key;
			changing_succeed = true;
		}
	}

	con_assert( changing_succeed );
}

returning Input::is_key_released( Hashed_CString name ) const -> bool
{
	for ( s32 i = 0; i < bindings.size(); ++i ) {
		auto& binding = bindings[i];

		if ( binding.name_hash == name.hash ) {
			return binding.released;
		}
	}

	con_push_indent();
	con_log( "Warning: couldn't find binding for key of hash %. (is_key_released)", name.hash );
	con_pop_indent();

	return false;
}

returning Input::is_key_pressed( Hashed_CString name ) const -> bool
{
	for ( s32 i = 0; i < bindings.size(); ++i ) {
		auto& binding = bindings[i];

		if ( binding.name_hash == name.hash ) {
			return binding.pressed;
		}
	}

	con_push_indent();
	con_log( "Warning: couldn't find binding for key of hash %. (is_key_pressed)", name.hash );
	con_pop_indent();

	return false;
}

returning Input::is_key_held( Hashed_CString name ) const -> bool
{
	con_push_indent();
	defer{ con_pop_indent(); };

	for ( s32 i = 0; i < bindings.size(); ++i ) {
		auto& binding = bindings[i];

		if ( binding.name_hash == name.hash ) {

			constant glfw_key_id = con_key_to_glfw_key( binding.key_id );
			if ( binding.key_id > CON_INPUT_GAMEPAD_KEYS_OFFSET ) {
				con_log( "Warning: gamepad keys are not supported!" );
			} else if ( binding.key_id > CON_INPUT_MOUSE_KEYS_OFFSET ) {
				return glfwGetMouseButton( window_handle, glfw_key_id );
			} else {
				return glfwGetKey( window_handle, glfw_key_id );
			}
		}
	}

	con_log( "Warning: couldn't find binding for key of hash %. (is_key_held)", name.hash );

	return false;
}

returning Input::get_mouse_position() const -> v2
{
	f64 x = -1, y = -1;
	glfwGetCursorPos( window_handle, &x, &y );

	return { static_cast<f32>( x ), static_cast<f32>( y ) };
}

void Input::poll_events()
{
	for ( s32 i = 0; i < bindings.size(); ++i ) {
		auto& binding = bindings[i];

		binding.pressed = binding.released = false;
	}

	glfwPollEvents();
}

void Input::glfw_keyboard_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	unused( mods );
	unused( window );

	con_push_indent();
	defer{ con_pop_indent(); };

	Key_ID our_key = -1;
	if ( key == GLFW_KEY_UNKNOWN ) {
		con_log( "Error: unknown keyboard key (GLFW code: % / scancode: %).", key, scancode );
		return;
	}

	our_key = glfw_keyboard_key_to_con_key( key );

	auto& input = *Context.input;

	for ( s32 i = 0; i < input.bindings.size(); ++i ) {
		auto& binding = input.bindings[i];

		if ( binding.key_id == our_key ) {
			binding.released = action == GLFW_RELEASE;
			binding.pressed =  action == GLFW_PRESS;
		}
	}
}

void Input::glfw_mouse_key_callback( GLFWwindow* window, int button, int action, int mods )
{
	unused( mods );
	unused( window );

	con_push_indent();
	defer{ con_pop_indent(); };

	Key_ID our_key = -1;
	if ( button == GLFW_KEY_UNKNOWN ) {
		con_log( " Error: unknown mouse button (GLFW code: %i).", button );
		return;
	}

	our_key = glfw_mouse_key_to_con_key( button );

	auto& input = *Context.input;

	for ( s32 i = 0; i < input.bindings.size(); ++i ) {
		auto& binding = input.bindings[i];

		if ( binding.key_id == our_key ) {
			binding.released = action == GLFW_RELEASE;
			binding.pressed =  action == GLFW_PRESS;
		}
	}
}

void Input::glfw_mouse_scroll_callback( GLFWwindow* window, double x_offset, double y_offset )
{
	unused( window );
	unused( x_offset );

	auto& dev_console = *Context.dev_console;

	if ( y_offset > 0 ) {
		dev_console.scroll_up();
	} else if ( y_offset < 0 ) {
		dev_console.scroll_down();
	}
}
}