#include <cstdlib>
#include <iostream>

#include <engine/application.hpp>

//
// Give windows a hint that we want to use dedicated GPU. 
// Using dedicated GPU also gives us more OpenGL messages.
//
extern "C"
{
	__declspec( dllexport ) con::u32 AmdPowerXpressRequestHighPerformance = CON_REQUEST_DEDICATED_GPU ? 1 : 0;
	__declspec( dllexport ) con::u32 NvOptimusEnablement = CON_REQUEST_DEDICATED_GPU ? 1 : 0;
}

//
//	Use this project only as a entry point. We want all of our code to be in the engine, actually.
//
// @ToDo: Use WinMain in release mode.
int main()
{
	std::ios_base::sync_with_stdio( false );
	// @Robustness: std::getnv caused a hang/lag in the release build? (17 June 2020)
	// std::printf( "AppData path = %s", std::getenv( "appdata" ) );

	con::Application app;
	// @ToDo: Better error message (dedicated error box instead of assert?)
	if ( app.initialize() ) {
		app.run();
	} else {
		con_log( "Initialization failed!" );
	}
	app.shutdown();
}