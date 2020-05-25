#include <catch/catch.hpp>

#include <engine/logger.hpp>
#include <engine/allocator.hpp>

TEST_CASE( "logger", "engine" )
{
	using namespace con;

	Default_Allocator da;
	Temporary_Allocator ta;
	Context.default_allocator = &da;
	Context.temporary_storage_allocator = &ta;
	da.initialize();
	ta.initialize();

	Logger l;
	l.set_instance( &l );
	l.initialize();

	con_log( "[LOGGER TEST BEGIN]" );
	con_log( "My_Value = %. Other value = %. String = %.", 69420, 21.37, "I'm a string!"_cs );
	con_log_indented( 1, "<== this should be indented!!. arg: %", "Is it indented?"_cs );
	con_log_indented( 2, "<== another indent." );
	con_log( "val = %. This should be normal!", 321 );
	con_log( "[LOGGER TEST END]" );

	constant buffer = G_Logger.get_buffer();

	// Hack: we know that buffer.data[buffer.size] is a valid memory so we put \0 there.
	// This way we don't have to call putchar in a loop.
	const_cast<char*>(buffer.data)[buffer.size] = '\0';
	puts( buffer.data );

	printf( "Memory used in temporary buffer by logger test: %i/%i.\n\n", ta.get_mark(), CON_TEMPORARY_STORAGE_RESERVED_MEMORY );

	da.shutdown();
	
	Context.default_allocator = Context.temporary_storage_allocator = nullptr;
}