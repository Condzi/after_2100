#include <cstdlib>
#include <iostream>

int main()
{
	std::printf( "AppData path = %s", std::getenv( "appdata" ) );
}