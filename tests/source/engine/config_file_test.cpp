#include <catch\catch.hpp>

#include <engine/config_file.hpp>

TEST_CASE( "config_file", "engine" )
{
	printf( "\n\nConfig file test, file: \"%s\".\n\n", CON_CONFIG_FILE );
	using namespace con;

	Default_Allocator da;
	Temporary_Allocator ta;
	Context.default_allocator = &da;
	Context.temporary_allocator = &ta;
	da.initialize();
	ta.initialize();

	{
		Config_File cfg;
		cfg.parse_from_file( CON_CONFIG_FILE );

		CString const vsync = "true";
		CString const window_size = "1920 1080";
		CString const dev_console = "false";
		constant vsync_ = cfg.get_value( "display"_hcs, "vsync"_hcs );
		constant window_size_ = cfg.get_value( "display"_hcs, "window_size"_hcs );
		constant dev_console_ = cfg.get_value( "debug"_hcs, "dev_console"_hcs );

		con_assert( vsync == vsync_ );
		con_assert( window_size == window_size_ );
		con_assert( dev_console == dev_console_ );

		cfg.free();
	}

	da.shutdown();
	Context.default_allocator = Context.temporary_allocator = nullptr;
}