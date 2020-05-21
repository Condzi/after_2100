#define CATCH_CONFIG_DISABLE_EXCEPTIONS

#if 1
#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#else
#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

#include <engine/assert.hpp>

int main()
{
	conassert( 550555==0 );
}
#endif