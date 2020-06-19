#define CATCH_CONFIG_DISABLE_EXCEPTIONS

#if 1
#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#else
#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

#include <engine/config_file.hpp>

using namespace con;

void print_cstring( CString str )
{
	for ( s32 i = 0; i < str.size; ++i ) {
		putc( str.data[i], stdout );
	}
}

int main()
{
	Default_Allocator da;
	Temporary_Allocator ta;
	Context.default_allocator = &da;
	Context.temporary_allocator = &ta;
	da.initialize();
	ta.initialize();

	Config_File cfg;
	cfg.parse_from_file( CON_CONFIG_FILE );

	CString const vsync = "true";
	CString const window_size = "1920 1080";
	CString const dev_console = "false";
	constant vsync_ = cfg.get_value( "display", "vsync" );
	constant window_size_ = cfg.get_value( "display", "window_size" );
	constant dev_console_ = cfg.get_value( "debug", "dev_console" );

	con_assert( vsync == vsync_ );
	con_assert( window_size == window_size_ );
	con_assert( dev_console == dev_console_ );

}
#endif