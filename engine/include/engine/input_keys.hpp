#pragma once

#include <engine/typedefs.hpp>

#include <GLFW/glfw3.h>

namespace con
{
// Mouse or keyboard key. -1 is invalid one.
using Key_ID = s16;

struct Key final
{
	enum _ : s16
	{
		Unknown= -1,

		//
		//	Keyboard Keys
		//

		/* Printable keys */
		Keyboard_SPACE              = GLFW_KEY_SPACE,
		Keyboard_APOSTROPHE         = GLFW_KEY_APOSTROPHE,  /* ' */
		Keyboard_COMMA              = GLFW_KEY_COMMA,		/* , */
		Keyboard_MINUS              = GLFW_KEY_MINUS,		/* - */
		Keyboard_PERIOD             = GLFW_KEY_PERIOD,		/* . */
		Keyboard_SLASH              = GLFW_KEY_SLASH,		/* / */
		Keyboard_0                  = GLFW_KEY_0,
		Keyboard_1                  = GLFW_KEY_1,
		Keyboard_2                  = GLFW_KEY_2,
		Keyboard_3                  = GLFW_KEY_3,
		Keyboard_4                  = GLFW_KEY_4,
		Keyboard_5                  = GLFW_KEY_5,
		Keyboard_6                  = GLFW_KEY_6,
		Keyboard_7                  = GLFW_KEY_7,
		Keyboard_8                  = GLFW_KEY_8,
		Keyboard_9                  = GLFW_KEY_9,
		Keyboard_SEMICOLON          = GLFW_KEY_SEMICOLON,   /* ; */
		Keyboard_EQUAL              = GLFW_KEY_EQUAL,		/* = */
		Keyboard_A                  = GLFW_KEY_A,
		Keyboard_B                  = GLFW_KEY_B,
		Keyboard_C                  = GLFW_KEY_C,
		Keyboard_D                  = GLFW_KEY_D,
		Keyboard_E                  = GLFW_KEY_E,
		Keyboard_F                  = GLFW_KEY_F,
		Keyboard_G                  = GLFW_KEY_G,
		Keyboard_H                  = GLFW_KEY_H,
		Keyboard_I                  = GLFW_KEY_I,
		Keyboard_J                  = GLFW_KEY_J,
		Keyboard_K                  = GLFW_KEY_K,
		Keyboard_L                  = GLFW_KEY_L,
		Keyboard_M                  = GLFW_KEY_M,
		Keyboard_N                  = GLFW_KEY_N,
		Keyboard_O                  = GLFW_KEY_O,
		Keyboard_P                  = GLFW_KEY_P,
		Keyboard_Q                  = GLFW_KEY_Q,
		Keyboard_R                  = GLFW_KEY_R,
		Keyboard_S                  = GLFW_KEY_S,
		Keyboard_T                  = GLFW_KEY_T,
		Keyboard_U                  = GLFW_KEY_U,
		Keyboard_V                  = GLFW_KEY_V,
		Keyboard_W                  = GLFW_KEY_W,
		Keyboard_X                  = GLFW_KEY_X,
		Keyboard_Y                  = GLFW_KEY_Y,
		Keyboard_Z                  = GLFW_KEY_Z,
		Keyboard_LEFT_BRACKET       = GLFW_KEY_LEFT_BRACKET,  /* [ */
		Keyboard_BACKSLASH          = GLFW_KEY_BACKSLASH,     /* \ */
		Keyboard_RIGHT_BRACKET      = GLFW_KEY_RIGHT_BRACKET,  /* ] */
		Keyboard_GRAVE_ACCENT       = GLFW_KEY_GRAVE_ACCENT,  /* ` */
		Keyboard_WORLD_1            = GLFW_KEY_WORLD_1, /* non-US #1 */
		Keyboard_WORLD_2            = GLFW_KEY_WORLD_2, /* non-US #2 */
		Keyboard_ESCAPE             = GLFW_KEY_ESCAPE,
		Keyboard_ENTER              = GLFW_KEY_ENTER,
		Keyboard_TAB                = GLFW_KEY_TAB,
		Keyboard_BACKSPACE          = GLFW_KEY_BACKSPACE,
		Keyboard_INSERT             = GLFW_KEY_INSERT,
		Keyboard_DELETE             = GLFW_KEY_DELETE,
		Keyboard_RIGHT              = GLFW_KEY_RIGHT,
		Keyboard_LEFT               = GLFW_KEY_LEFT,
		Keyboard_DOWN               = GLFW_KEY_DOWN,
		Keyboard_UP                 = GLFW_KEY_UP,
		Keyboard_PAGE_UP            = GLFW_KEY_PAGE_UP,
		Keyboard_PAGE_DOWN          = GLFW_KEY_PAGE_DOWN,
		Keyboard_HOME               = GLFW_KEY_HOME,
		Keyboard_END                = GLFW_KEY_END,
		Keyboard_CAPS_LOCK          = GLFW_KEY_CAPS_LOCK,
		Keyboard_SCROLL_LOCK        = GLFW_KEY_SCROLL_LOCK,
		Keyboard_NUM_LOCK           = GLFW_KEY_NUM_LOCK,
		Keyboard_PRINT_SCREEN       = GLFW_KEY_PRINT_SCREEN,
		Keyboard_PAUSE              = GLFW_KEY_PAUSE,
		Keyboard_F1                 = GLFW_KEY_F1,
		Keyboard_F2                 = GLFW_KEY_F2,
		Keyboard_F3                 = GLFW_KEY_F3,
		Keyboard_F4                 = GLFW_KEY_F4,
		Keyboard_F5                 = GLFW_KEY_F5,
		Keyboard_F6                 = GLFW_KEY_F6,
		Keyboard_F7                 = GLFW_KEY_F7,
		Keyboard_F8                 = GLFW_KEY_F8,
		Keyboard_F9                 = GLFW_KEY_F9,
		Keyboard_F10                = GLFW_KEY_F10,
		Keyboard_F11                = GLFW_KEY_F11,
		Keyboard_F12                = GLFW_KEY_F12,
		Keyboard_F13                = GLFW_KEY_F13,
		Keyboard_F14                = GLFW_KEY_F14,
		Keyboard_F15                = GLFW_KEY_F15,
		Keyboard_F16                = GLFW_KEY_F16,
		Keyboard_F17                = GLFW_KEY_F17,
		Keyboard_F18                = GLFW_KEY_F18,
		Keyboard_F19                = GLFW_KEY_F19,
		Keyboard_F20                = GLFW_KEY_F20,
		Keyboard_F21                = GLFW_KEY_F21,
		Keyboard_F22                = GLFW_KEY_F22,
		Keyboard_F23                = GLFW_KEY_F23,
		Keyboard_F24                = GLFW_KEY_F24,
		Keyboard_F25                = GLFW_KEY_F25,
		Keyboard_KP_0               = GLFW_KEY_KP_0,
		Keyboard_KP_1               = GLFW_KEY_KP_1,
		Keyboard_KP_2               = GLFW_KEY_KP_2,
		Keyboard_KP_3               = GLFW_KEY_KP_3,
		Keyboard_KP_4               = GLFW_KEY_KP_4,
		Keyboard_KP_5               = GLFW_KEY_KP_5,
		Keyboard_KP_6               = GLFW_KEY_KP_6,
		Keyboard_KP_7               = GLFW_KEY_KP_7,
		Keyboard_KP_8               = GLFW_KEY_KP_8,
		Keyboard_KP_9               = GLFW_KEY_KP_9,
		Keyboard_KP_DECIMAL         = GLFW_KEY_KP_DECIMAL,
		Keyboard_KP_DIVIDE          = GLFW_KEY_KP_DIVIDE,
		Keyboard_KP_MULTIPLY        = GLFW_KEY_KP_MULTIPLY,
		Keyboard_KP_SUBTRACT        = GLFW_KEY_KP_SUBTRACT,
		Keyboard_KP_ADD             = GLFW_KEY_KP_ADD,
		Keyboard_KP_ENTER           = GLFW_KEY_KP_ENTER,
		Keyboard_KP_EQUAL           = GLFW_KEY_KP_EQUAL,
		Keyboard_LEFT_SHIFT         = GLFW_KEY_LEFT_SHIFT,
		Keyboard_LEFT_CONTROL       = GLFW_KEY_LEFT_CONTROL,
		Keyboard_LEFT_ALT           = GLFW_KEY_LEFT_ALT,
		Keyboard_LEFT_SUPER         = GLFW_KEY_LEFT_SUPER,
		Keyboard_RIGHT_SHIFT        = GLFW_KEY_RIGHT_SHIFT,
		Keyboard_RIGHT_CONTROL      = GLFW_KEY_RIGHT_CONTROL,
		Keyboard_RIGHT_ALT          = GLFW_KEY_RIGHT_ALT,
		Keyboard_RIGHT_SUPER        = GLFW_KEY_RIGHT_SUPER,
		Keyboard_MENU               = GLFW_KEY_MENU,


		// 
		// Mouse Keys (offset by the last keyboard key)
		//
		Mouse_1      = GLFW_MOUSE_BUTTON_1 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_2      = GLFW_MOUSE_BUTTON_2 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_3      = GLFW_MOUSE_BUTTON_3 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_4      = GLFW_MOUSE_BUTTON_4 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_5      = GLFW_MOUSE_BUTTON_5 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_6      = GLFW_MOUSE_BUTTON_6 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_7      = GLFW_MOUSE_BUTTON_7 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_8      = GLFW_MOUSE_BUTTON_8 + CON_INPUT_MOUSE_KEYS_OFFSET,
		Mouse_LEFT   = Mouse_1,
		Mouse_RIGHT  = Mouse_2,
		Mouse_MIDDLE = Mouse_3,

		// @ToDo: No analog support yet.
		//
		//	Gamepad Keys (offset by last mouse key)
		//
		Gamepad_A            = GLFW_GAMEPAD_BUTTON_A            + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_B            = GLFW_GAMEPAD_BUTTON_B            + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_X            = GLFW_GAMEPAD_BUTTON_X            + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_Y            = GLFW_GAMEPAD_BUTTON_Y            + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_LEFT_BUMPER  = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER  + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_BACK         = GLFW_GAMEPAD_BUTTON_BACK         + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_START        = GLFW_GAMEPAD_BUTTON_START        + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_GUIDE        = GLFW_GAMEPAD_BUTTON_GUIDE        + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_LEFT_THUMB   = GLFW_GAMEPAD_BUTTON_LEFT_THUMB   + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_RIGHT_THUMB  = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB  + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_DPAD_UP      = GLFW_GAMEPAD_BUTTON_DPAD_UP      + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_DPAD_RIGHT   = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT   + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_DPAD_DOWN    = GLFW_GAMEPAD_BUTTON_DPAD_DOWN    + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_DPAD_LEFT    = GLFW_GAMEPAD_BUTTON_DPAD_LEFT    + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_CROSS		 = GLFW_GAMEPAD_BUTTON_CROSS        + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_CIRCLE		 = GLFW_GAMEPAD_BUTTON_CIRCLE       + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_SQUARE		 = GLFW_GAMEPAD_BUTTON_SQUARE       + CON_INPUT_GAMEPAD_KEYS_OFFSET,
		Gamepad_TRIANGLE	 = GLFW_GAMEPAD_BUTTON_TRIANGLE     + CON_INPUT_GAMEPAD_KEYS_OFFSET
	};
};

returning constexpr con_key_to_glfw_key( Key_ID con_id ) -> s32
{
	if ( con_id >= Key::Gamepad_A ) {
		return con_id - CON_INPUT_GAMEPAD_KEYS_OFFSET;
	} else if ( con_id >= Key::Mouse_1 ) {
		return con_id - CON_INPUT_MOUSE_KEYS_OFFSET;
	} else {
		return con_id;
	}
}

returning constexpr glfw_keyboard_key_to_con_key( s32 glfw_keyboard_key ) -> Key_ID
{
	return static_cast<Key_ID>( glfw_keyboard_key );
}

returning constexpr glfw_mouse_key_to_con_key( s32 glfw_mouse_key ) -> Key_ID
{
	return static_cast<Key_ID>( glfw_mouse_key ) + CON_INPUT_MOUSE_KEYS_OFFSET;
}
}