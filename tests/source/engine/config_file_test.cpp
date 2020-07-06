#include <testing_utilities.hpp>
#include <engine/config_file.hpp>
#include <engine/algorithms.hpp>

TEST_CASE( "config_file", "engine" )
{
	printf( "\n\nConfig file test, file: \"%s\".\n\n", CON_CONFIG_FILE );
	using namespace con;

	Scoped_Test_Initializer sti{};

	{
		Config_File cfg;
		cfg.parse_from_file( CON_CONFIG_FILE );

		CString const vsync		  = "true";
		CString const window_size = "1920 1080";
		CString const dev_console = "false";

		constant vsync_		  = cfg.get_value( "display"_hcs, "vsync"_hcs );
		constant window_size_ = cfg.get_value( "display"_hcs, "window_size"_hcs );
		constant dev_console_ = cfg.get_value( "debug"_hcs, "dev_console"_hcs );

		REQUIRE( vsync == vsync_ );
		REQUIRE( window_size == window_size_ );
		REQUIRE( dev_console == dev_console_ );


		constant display_hash  = "display"_hcs.hash;
		constant gameplay_hash = "gameplay"_hcs.hash;
		constant debug_hash    = "debug"_hcs.hash;

		constant sections_array = cfg.get_all_section_hashes();

		REQUIRE( sections_array.size() == 3 );

		REQUIRE( linear_find( sections_array, display_hash ).found()  == true );
		REQUIRE( linear_find( sections_array, gameplay_hash ).found() == true );
		REQUIRE( linear_find( sections_array, debug_hash ).found()	  == true );


		cfg.free();
	}
}