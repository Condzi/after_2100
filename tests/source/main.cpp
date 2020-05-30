#define CATCH_CONFIG_DISABLE_EXCEPTIONS

#if 1
#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#else
#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

#include <engine/entity_base.hpp>

int main()
{
	printf( "entity_base = %i", sizeof( con::Entity_Base ) );
}
#endif