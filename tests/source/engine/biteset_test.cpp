#include <testing_utilities.hpp>

#include <engine/allocator.hpp>
#include <engine/bitset.hpp>

TEST_CASE( "bitset", "engine" )
{
	using namespace con;

	Scoped_Test_Initializer sti{};

	compile_constant bitset_count = 33;
	Bitset bitset;

	bitset.initialize( bitset_count );

	// Set 1st, 17th and last bit and check if all operations have correct
	// outcomes.
	{
		bitset.set( 0 );
		bitset.set( 17 );
		bitset.set( bitset_count - 1 );

		REQUIRE( bitset.count_set_bits() == 3 );
		REQUIRE( bitset.test( 0 ) );
		REQUIRE( bitset.test( 17 ) );
		REQUIRE( bitset.test( bitset_count - 1 ) );

		REQUIRE( bitset.find_first_unset_bit().idx == 1 );
		REQUIRE( bitset.find_first_unset_bit( 17 ).idx == 18 );
		REQUIRE( bitset.find_first_unset_bit( bitset_count - 1 ).not_found() == true );
	}

	// Flip 1st, 17th and last bit (so we should have clear bitset now)
	{
		bitset.flip( 0 );
		bitset.flip( 17 );
		bitset.flip( bitset_count - 1 );

		REQUIRE( bitset.count_set_bits() == 0 );
		REQUIRE( !bitset.test( 0 ) );
		REQUIRE( !bitset.test( 17 ) );
		REQUIRE( !bitset.test( bitset_count - 1 ) );

		REQUIRE( bitset.find_first_unset_bit().idx == 0 );
		REQUIRE( bitset.find_first_unset_bit( 17 ).idx == 17 );
		REQUIRE( bitset.find_first_unset_bit( bitset_count - 1 ).idx == bitset_count - 1 );
	}


	// 1. Set two ranges: <1, 13> and <22, 30>
	// 2. Reset range <10, 25> => we end up with <1,9> and <26, 30>
	// 3. Set range <4, 6> => we end up with <1, 9> and <26, 30> (still)
	// 4. Reset range <1, 9> and <26, 30> (so we should've clear bitset now)
	{
		// 1. begin
		bitset.set_range( 1, 13 );

		for ( s32 i = 1; i < 14; ++i ) {
			REQUIRE( bitset.test( i ) );
		}

		REQUIRE( bitset.count_set_bits() == 13 );
		REQUIRE( bitset.find_first_unset_bit().idx == 0 );
		REQUIRE( bitset.find_first_unset_bit( 5 ).idx == 14 );

		bitset.set_range( 22, 9 );

		for ( s32 i = 22; i < 31; ++i ) {
			REQUIRE( bitset.test( i ) );
		}

		REQUIRE( bitset.count_set_bits() == 22 );
		REQUIRE( bitset.find_first_unset_bit().idx == 0 );
		REQUIRE( bitset.find_first_unset_bit( 5 ).idx == 14 );
		REQUIRE( bitset.find_first_unset_bit( 25 ).idx == 31 );

		// 1. end, 2. begin
		bitset.reset_range( 10, 16 );

		REQUIRE( bitset.count_set_bits() == 14 );
		REQUIRE( bitset.find_first_unset_bit().idx == 0 );
		REQUIRE( bitset.find_first_unset_bit( 5 ).idx == 10 );
		REQUIRE( bitset.find_first_unset_bit( 26 ).idx == 31 );

		// 2. end, 3. begin
		bitset.set_range( 4, 3 );

		REQUIRE( bitset.count_set_bits() == 14 );
		REQUIRE( bitset.find_first_unset_bit().idx == 0 );
		REQUIRE( bitset.find_first_unset_bit( 5 ).idx == 10 );
		REQUIRE( bitset.find_first_unset_bit( 26 ).idx == 31 );

		// 3. end, 4. begin
		bitset.reset_range( 1, 9 );
		bitset.reset_range( 26, 5 );

		REQUIRE( bitset.count_set_bits() == 0 );
		REQUIRE( bitset.find_first_unset_bit().idx == 0 );
		REQUIRE( bitset.find_first_unset_bit( 5 ).idx == 5 );
		REQUIRE( bitset.find_first_unset_bit( 26 ).idx == 26 );

		// 4. end
	}

	bitset.shutdown();
}