#include <testing_utilities.hpp>

#include <engine/logger.hpp>
#include <engine/allocator.hpp>

TEST_CASE( "logger", "engine" )
{
	using namespace con;

	Scoped_Test_Initializer sti{};

	Logger l;
	l.initialize();
	Context.logger = &l;

	con_log( "[LOGGER TEST BEGIN]" );
	con_log( "My_Value = %. Other value = %. String = %.", 69420, 21.37, "I'm a string!"_cs );
	con_log_indented( 1, "<== this should be indented!!. arg: %", "Is it indented?"_cs );
	con_log_indented( 2, "<== another indent." );
	con_log( "val = %. This should be normal!", 321 );
	con_log( "[LOGGER TEST END]" );

	constant buffer = Context.logger->get_buffer();

	puts( buffer.data );

	printf( "Memory used in temporary buffer by logger test: %i/%i.\n\n", reinterpret_cast<Temporary_Allocator&>( *Context.temporary_allocator ).get_mark(), CON_TEMPORARY_STORAGE_RESERVED_MEMORY );

	l.shutdown();
}