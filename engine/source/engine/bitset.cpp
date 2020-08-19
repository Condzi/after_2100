#include <engine/allocator.hpp>
#include <engine/bitset.hpp>

#include <cstring>
#include <intrin.h>

//
// Wrappers for MSVC intrinsics.
//
namespace con::priv
{
returning count_set_bits( u64 value ) -> s32
{
	return static_cast<s32>( static_cast<unsigned __int64>( value ) );
}

returning find_first_unset_bit( u64 value ) -> Find_Result
{
	if ( value == 0 ){
		return { .idx = 0 };
	}

	unsigned long idx = 0;
	auto value_casted =  static_cast<unsigned __int64>( value );
	// We have to negate it because _BitScanForward looks for
	// first set bit -- we need first unset one.
	value_casted = ~value_casted;

	if ( _BitScanForward64( &idx, value_casted ) == 0 ){
		return { .idx = -1 };
	}

	return { .idx = static_cast<s32>( idx ) };
}
}

namespace con
{
void Bitset::initialize( s32 size_, Allocator* allocator_ )
{
	con_assert( size_ > 0 );
	con_assert( allocator_ != nullptr );
	// Fires if Bitset was already initialized. Or we have garbage in the memory.
	con_assert( data == nullptr );

	allocator          = allocator_;
	size_in_bits       = size_;
	size_in_base_types = ( size_in_bits / base_type_size_in_bits ) + 1;

	data = reinterpret_cast<u64*>( allocator->allocate( base_type_size_in_bytes * size_in_base_types ) );

	memset( data, 0, base_type_size_in_bytes * size_in_base_types );
}

void Bitset::shutdown()
{
	if ( data == nullptr ){
		return;
	}

	allocator->free( reinterpret_cast<byte*>( data ), base_type_size_in_bytes * size_in_base_types );

	data               = nullptr;
	allocator          = nullptr;
	size_in_bits       = -1;
	size_in_base_types = -1;
}

void Bitset::set( s32 idx )
{
	con_assert( idx >= 0 );
	con_assert( idx < size_in_bits );

	data[idx / base_type_size_in_bits] |= ( 1ull << ( idx % base_type_size_in_bits ) );
}

void Bitset::reset( s32 idx )
{
	con_assert( idx >= 0 );
	con_assert( idx < size_in_bits );

	data[idx / base_type_size_in_bits] &= ~( 1ull << ( idx%base_type_size_in_bits ) );
}

void Bitset::set_range( s32 idx, s32 size )
{
	con_assert( idx >= 0 );
	con_assert( idx + size < size_in_bits );

	// @Performance: set every u64 instead of individual bits.
	// We don't need such micro-optimisation right now. -- condzi, 12.08.2020

	for ( s32 i = idx; i < idx + size; ++i ){
		data[i / base_type_size_in_bits] |= ( 1ull << ( i % base_type_size_in_bits ) );
	}
}

void Bitset::reset_range( s32 idx, s32 size )
{
	con_assert( idx >= 0 );
	con_assert( idx + size < size_in_bits );

	for ( s32 i = idx; i < idx + size; ++i ){
		data[i / base_type_size_in_bits] &= ~( 1ull << ( i%base_type_size_in_bits ) );
	}
}

void Bitset::flip( s32 idx )
{
	con_assert( idx >= 0 );
	con_assert( idx < size_in_bits );

	data[idx / base_type_size_in_bits] ^= ( 1ull << ( idx%base_type_size_in_bits ) );
}

void Bitset::clear()
{
	memset( data, 0, base_type_size_in_bytes * size_in_base_types );
}

returning Bitset::test( s32 idx ) const -> bool
{
	con_assert( idx >= 0 );
	con_assert( idx < size_in_bits );

	return data[idx / base_type_size_in_bits] & ( 1ull << ( idx%base_type_size_in_bits ) );
}

returning Bitset::find_first_unset_bit( s32 begin ) const -> Find_Result
{
	// We loop until we get to k * BASE_TYPE_SIZE_IN_BITS, then we can safelly
	// use the find_first_unset_bit function.

	s32 bit = begin;
	for ( ; bit % base_type_size_in_bits != 0 && bit < size_in_bits; ++bit ) {
		if ( test( bit ) == false ) {
			return { .idx = bit };
		}
	}

	if ( bit >= size_in_bits-1 ) {
		return { .idx = -1 };
	}

	s32 u64_idx = bit / base_type_size_in_bits;
	for ( ; u64_idx < size_in_base_types; ++u64_idx ) {
		constant result = con::priv::find_first_unset_bit( data[u64_idx] );

		if ( result.found() ) {
			// We may have exceed declared size in bits (but we never do that with size_in_base_types)
			// so we have to check for that.
			constant bit_to_return = u64_idx * base_type_size_in_bits + result.idx;
			if ( bit_to_return >= size_in_bits ) {
				return { .idx = -1 };
			} else {
				return { .idx = bit_to_return };
			}
		}
	}

	return { .idx = -1 };
}

returning Bitset::count_set_bits() const -> s32
{
	s32 sum = 0;

	for ( s32 i = 0; i < size_in_base_types; ++i ) {
		sum += con::priv::count_set_bits( data[i] );
	}

	return sum;
}
}