#pragma once

#define CON_BYTES( x )		 x
#define CON_KILOBYTES( x )	 CON_BYTES( x ) * 1000
#define CON_MEGABYTES( x )	 CON_KILOBYTES( x ) * 1000

#define CON_VERSION_STRING "1.0"

#define CON_RESERVED_MEMORY  CON_MEGABYTES( 64 )
#define CON_TEMPORARY_STORAGE_RESERVED_MEMORY  CON_MEGABYTES( 8 )

#define CON_GL_DEBUG false

#ifdef NDEBUG
#define CON_DEBUG false
#else
#define CON_DEBUG true
#endif

//
// Paths relative to run tree.
//
#define CON_LOGS_FOLDER			"logs/"
#define CON_DATA_FOLDER			"data/"
#define CON_ASSETS_FOLDER		CON_DATA_FOLDER "assets/"
#define CON_TEXTURES_FOLDER		CON_ASSETS_FOLDER "textures/"