#include <engine/bitset.hpp>

#include <intrin.h>

namespace con
{
returning count_set_bits( u16 value ) -> s32
{
	// msvc specific
	return static_cast<s32>( __popcnt16( static_cast<unsigned short>( value ) ) );
}

returning find_first_unset_bit( u16 value ) -> Find_Result
{
	if ( value == 0 ) {
		return { .idx = 0 };
	}

	unsigned long idx = 0;


	/*
	 First, we convert u16 value to u32

			11111111111111110000000000000000 or
			00000000000000001000000000000000 =
			11111111111111111000000000000000

	 Then we negate it to make it work with _BitScan which
	 looks for first *set* bit (we're looking for unset one):

			11111111111111111000000000000000 not
			00000000000000000111111111111111
	*/
	compile_constant mask = 0xFFFF0000; /* u32( 0b11111111111111110000000000000000 ) */
	constant value_to_check = ~( static_cast<unsigned long>( value ) | mask );

	if ( _BitScanForward( &idx, value_to_check ) == 0 ) {
		return { .idx  = -1 };
	}

	return { .idx = static_cast<s32>( idx ) };
}
}