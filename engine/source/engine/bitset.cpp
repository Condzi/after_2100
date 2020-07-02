#include <engine/bitset.hpp>

#include <intrin.h>

namespace con
{
returning count_set_bits( u16 value ) -> s32
{
	// msvc specific
	return static_cast<s32>( __popcnt16( static_cast<unsigned short>( value ) ) );
}
}