#include <cstdlib>
#include <iostream>

#include <engine/application.hpp>

//
//	Use this project only as a entry point. We want all of our code to be in the engine, actually.
//
// @ToDo: Use WinMain in release mode.
int main()
{
	using Init_Error_Code = con::Application::Initialization_Error_Code;
	std::ios_base::sync_with_stdio( false );
	std::printf( "AppData path = %s", std::getenv( "appdata" ) );

	con::Application app;

	constant init_error_code = app.initialize();
	// @ToDo: Better error message (dedicated error box instead of assert?)
	release_con_assert( init_error_code == Init_Error_Code::Success );
	app.run();
	app.shutdown();

	constant now = con::Time::now();
	printf( "\n\tEleapsed: %fs\n", con::Time::to_seconds( con::Time::difference( Context.startup_time_point, now ) ) );
}