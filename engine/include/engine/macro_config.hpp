#pragma once

//
//	Helper macros
//

#define CON_STR_( x )  #x
#define CON_STR( x )   CON_STR_( x )

#define CON_CONCAT_INTERNAL( x, y )  x##y
#define CON_CONCAT( x, y )           CON_CONCAT_INTERNAL( x, y )

#define CON_BYTES( x )       ( x )
#define CON_KILOBYTES( x )   ( x << 10 )
#define CON_MEGABYTES( x )   ( x << 20 )


//
// Engine and game stuff
//

#ifdef NDEBUG
#define CON_DEBUG false
#else
#define CON_DEBUG true
#endif

#define CON_VERSION_STRING  "1.0"
#define CON_WINDOW_TITLE    "after_2100 / v" CON_VERSION_STRING " / Compiled on " __DATE__ " at " __TIME__ " CET"

#define CON_ASSERT_MESSAGE_BUFFER_SIZE 512

#define CON_RESERVED_MEMORY                     CON_MEGABYTES( 64 )
#define CON_TEMPORARY_STORAGE_RESERVED_MEMORY   CON_MEGABYTES( 4 )
#define CON_STACK_RESERVED_MEMORY               CON_KILOBYTES( 128 )
#define CON_LOGGER_BUFFER_RESERVED_MEMORY       CON_MEGABYTES( 1 )
#define CON_DEV_CONSOLE_BUFFER_RESERVED_MEMORY  CON_LOGGER_BUFFER_RESERVED_MEMORY

#define CON_REQUEST_DEDICATED_GPU  true

//
// Input
//

// We pack glfw keys to one enum so we have to offset them.
#define CON_INPUT_MOUSE_KEYS_OFFSET   ( con::Key::Keyboard_MENU + 1 )
#define CON_INPUT_GAMEPAD_KEYS_OFFSET ( con::Key::Mouse_8 + 1 )

#define CON_MAX_INPUT_BINDINGS  8

//
// Data files extensions
//
#define CON_LOG_FILE_EXTENSION              ".txt"
#define CON_CONFIG_FILE_EXTENSION           ".variables"
#define CON_SCENE_RESOURCES_FILE_EXTENSION  ".scene_resources"
#define CON_SHADERS_FILE_EXTENSION          ".shaders"

//
// Paths relative to run tree.
//
#define CON_LOGS_FOLDER          "logs\\"
#define CON_DATA_FOLDER          "data\\"

#define CON_ASSETS_FOLDER        CON_DATA_FOLDER   "assets\\"
#define CON_SCENES_FOLDER        CON_ASSETS_FOLDER "scenes\\"
#define CON_TEXTURES_FOLDER      CON_ASSETS_FOLDER "textures\\"
#define CON_FONTS_FOLDER         CON_ASSETS_FOLDER "fonts\\"
#define CON_SHADERS_FOLDER       CON_ASSETS_FOLDER "shaders\\"

#define CON_CONFIG_FILE          CON_DATA_FOLDER "local" CON_CONFIG_FILE_EXTENSION
#define CON_DEFAULT_LOG_FILE     CON_LOGS_FOLDER "log" CON_LOG_FILE_EXTENSION
#define CON_DEFAULT_SCENE_RESOURCES_INFO_FILE   CON_SCENES_FOLDER "default" CON_SCENE_RESOURCES_FILE_EXTENSION
#define CON_ASSETS_CONFIG_FILE   CON_DATA_FOLDER "assets" CON_CONFIG_FILE_EXTENSION
#define CON_PLANETS_CONFIG_FILE  CON_DATA_FOLDER "planets" CON_CONFIG_FILE_EXTENSION


//
// Entities stuff
//
#define CON_ENTITIES_LIMIT   128
#define CON_MAX_ENTITY_FLAGS 8

#define CON_DEBUG_ENTITIES_LIMIT  4
#define CON_PLANETS_LIMIT         8

//
// Config file stuff
//
#define CON_CONFIG_SECTION_MARK  ":/"
#define CON_CONFIG_COMMENT_MARK  "#"

//
// We're using this values to estimate how much space do we need at max. We do this to allocate
// only once and then "shrink" the array (by freeing the memory).
// 

#define CON_MAX_SCENES_IN_FOLDER  32
#define CON_MAX_CONFIG_ENTRIES    64

// Fallback texture color in range <0, 255>
#define CON_FALLBACK_TEXTURE_COLOR_R  255
#define CON_FALLBACK_TEXTURE_COLOR_G  28
#define CON_FALLBACK_TEXTURE_COLOR_B  221
#define CON_FALLBACK_TEXTURE_COLOR_A  255

#define CON_FALLBACK_TEXTURE_SIZE 64

