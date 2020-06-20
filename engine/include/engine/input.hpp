#pragma once

#include <engine/array.hpp>
#include <engine/macro_config.hpp>
#include <engine/typedefs.hpp>
#include <engine/hashed_cstring.hpp>
#include <engine/input_keys.hpp>

namespace con
{
struct Window;

class Input final
{
public:
	void initialize( Window& window );
	void shutdown();

	// Asserts if there is enough space for binding in the array
	void add_binding( Hashed_CString name, Key_ID key );
	// Asserts if binding 'name' is not found.
	void change_binding( Hashed_CString name, Key_ID key );

	returning is_key_released( Hashed_CString name ) -> bool;
	returning is_key_pressed( Hashed_CString name )  -> bool;
	returning is_key_held( Hashed_CString name )     -> bool;

	void poll_events();

private:
	struct Key_Binding final
	{
		u32 name_hash = 0;
		// One key may have more than one binding! (For example we could use
		// SPACE for shooting and for accepting an option in menu)
		Key_ID key_id = -1;

		bool released : 1 = false;
		bool pressed : 1 = false;
	};

	GLFWwindow* window_handle;
	Array<Key_Binding> bindings;

	static void glfw_keyboard_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods );
	static void glfw_mouse_key_callback( GLFWwindow* window, int button, int action, int mods );

};
}