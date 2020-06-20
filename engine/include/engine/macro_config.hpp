#pragma once

//
//	Helper macros
//

#define CON_STR_( x )  #x
#define CON_STR( x )  CON_STR_( x )

#define CON_CONCAT_INTERNAL( x, y )  x##y
#define CON_CONCAT( x, y )  CON_CONCAT_INTERNAL( x, y )

#define CON_BYTES( x )		 x
#define CON_KILOBYTES( x )	 CON_BYTES( x ) * 1000
#define CON_MEGABYTES( x )	 CON_KILOBYTES( x ) * 1000

// We pack glfw keys to one enum so we have to offset them.
#define CON_INPUT_MOUSE_KEYS_OFFSET ( con::Key::Keyboard_MENU + 1 )
#define CON_INPUT_GAMEPAD_KEYS_OFFSET ( con::Key::Mouse_8 + 1 )


//
// Engine and game stuff
//

#ifdef NDEBUG
#define CON_DEBUG false
#else
#define CON_DEBUG true
#endif

#define CON_VERSION_STRING "1.0"
#define CON_WINDOW_TITLE   "after_2100 / v" CON_VERSION_STRING

#define CON_RESERVED_MEMORY  CON_MEGABYTES( 64 )
#define CON_TEMPORARY_STORAGE_RESERVED_MEMORY  CON_MEGABYTES( 8 )
#define CON_LOGGER_BUFFER_RESERVED_MEMORY  CON_MEGABYTES( 1 )

#define CON_MAX_INPUT_BINDINGS  8

//
// Paths relative to run tree.
//
#define CON_LOGS_FOLDER			"logs\\"
#define CON_DATA_FOLDER			"data\\"
#define CON_ASSETS_FOLDER		CON_DATA_FOLDER "assets\\"
#define CON_TEXTURES_FOLDER		CON_ASSETS_FOLDER "textures\\"

#define CON_CONFIG_FILE			CON_DATA_FOLDER "local.variables"
#define CON_DEFAULT_LOG_FILE	CON_LOGS_FOLDER "log.txt"

//
// Entities stuff
//
#define CON_ENTITIES_COUNT 128
#define CON_MAX_ENTITY_FLAGS 8

//
// Config file stuff
//
#define CON_CONFIG_SECTION_MARK ":/"
#define CON_CONFIG_COMMENT_MARK "#"