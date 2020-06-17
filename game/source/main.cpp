#include <cstdlib>
#include <iostream>

//
//	Use this project only as a entry point. We want all of our code to be in the engine, actually.
//
int main()
{
	std::printf( "AppData path = %s", std::getenv( "appdata" ) );
}