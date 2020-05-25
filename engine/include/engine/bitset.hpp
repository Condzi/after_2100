#pragma once

#include <engine/assert.hpp>
#include <engine/typedefs.hpp>

#include <cstring>
#include <limits>

namespace con
{
// @Performance: Add find_first_unset_bit() -> idx method 
// because we're often using it that way and doing test(i) for
// everything kinda suck. We can check every byte instead of bit.
// @Important: Use unsigned types as TBaseType!!!
template <s32 TSize, typename TBaseType = byte>
class Bitset final
{
public:
	compile_constant SIZE = TSize;
	compile_constant BASE_TYPE_SIZE_IN_BITS = static_cast<s32>( sizeof( TBaseType ) * 8 );
	compile_constant SIZE_IN_BASE_TYPES = SIZE / BASE_TYPE_SIZE_IN_BITS + 1;

	pure set( s32 idx );
	pure reset( s32 idx );
	pure set_range( s32 idx, s32 size );
	pure reset_range( s32 idx, s32 size );

	pure flip( s32 idx );
	pure clear();
	returning test( s32 idx ) const -> bool;

	returning find_first_unset_bit( s32 begin = 0 )const -> s32;

private:
	TBaseType data[SIZE_IN_BASE_TYPES] = { 0 };
};


template <s32 TSize, typename TBaseType>
void Bitset<TSize, TBaseType>::set( s32 idx )
{
	con_assert( idx < SIZE );
	data[idx / BASE_TYPE_SIZE_IN_BITS] |= ( 1 << ( idx%BASE_TYPE_SIZE_IN_BITS ) );
}

template <s32 TSize, typename TBaseType>
void Bitset<TSize, TBaseType>::reset( s32 idx )
{
	con_assert( idx < SIZE );
	data[idx / BASE_TYPE_SIZE_IN_BITS] &= ~( 1 << ( idx%BASE_TYPE_SIZE_IN_BITS ) );
}

template <s32 TSize, typename TBaseType>
pure Bitset<TSize, TBaseType>::set_range( s32 idx, s32 size )
{
	// @Performance: we probably can do better than that (setting every byte instead of every individual
	// bit), but we don't need such microoptimalizations

	con_assert( idx + size < SIZE );
	for ( s32 i = idx; i < idx + size; ++i ) {
		data[i / BASE_TYPE_SIZE_IN_BITS] |= ( 1 << ( i%BASE_TYPE_SIZE_IN_BITS ) );
	}
}

template <s32 TSize, typename TBaseType>
void Bitset<TSize, TBaseType>::reset_range( s32 idx, s32 size )
{
	con_assert( idx + size < SIZE );
	for ( s32 i = idx; i < idx + size; ++i ) {
		data[i / BASE_TYPE_SIZE_IN_BITS] &= ~( 1 << ( i%BASE_TYPE_SIZE_IN_BITS ) );
	}
}

template <s32 TSize, typename TBaseType>
void Bitset<TSize, TBaseType>::flip( s32 idx )
{
	con_assert( idx < SIZE );
	data[idx/ BASE_TYPE_SIZE_IN_BITS] ^= ( 1 << idx%BASE_TYPE_SIZE_IN_BITS );
}

template <s32 TSize, typename TBaseType>
void Bitset<TSize, TBaseType>::clear()
{
	std::memset( data, 0, sizeof( TBaseType ) * SIZE_IN_BASE_TYPES );
}

template <s32 TSize, typename TBaseType>
auto Bitset<TSize, TBaseType>::test( s32 idx ) const -> bool
{
	con_assert( idx < SIZE );
	return ( data[idx / BASE_TYPE_SIZE_IN_BITS] & ( 1 << ( idx % BASE_TYPE_SIZE_IN_BITS ) ) );
}

template<s32 TSize, typename TBaseType>
returning Bitset<TSize, TBaseType>::find_first_unset_bit( s32 begin ) const -> s32
{
	for ( s32 i = begin; i < SIZE_IN_BASE_TYPES; ++i ) {
		// If all bits are set, continue.
		if ( data[i] == std::numeric_limits<TBaseType>::min() - 1 ) {
			continue;
		}
		
		for ( s32 bit = i*BASE_TYPE_SIZE_IN_BITS; true; ++bit ) {
			if ( test( bit ) == false ) {
				return bit;
			}
		}
	}

	return -1;
}
}
