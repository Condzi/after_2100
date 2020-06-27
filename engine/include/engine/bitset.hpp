#pragma once

#include <engine/assert.hpp>
#include <engine/typedefs.hpp>

#include <cstring>
#include <limits>

namespace con
{
// @Robustness: Use initialize and shutdown and allocator instead of templated size.
// @Performance: Add find_first_unset_bit() -> idx method 
// because we're often using it that way and doing test(i) for
// everything kinda suck. We can check every byte instead of bit.
// @Important: Use unsigned types as TBaseType!!!
template <typename TBaseType = byte>
class Bitset final
{
public:
	compile_constant BASE_TYPE_SIZE_IN_BYTES = static_cast<s32>( sizeof( TBaseType ) );
	compile_constant BASE_TYPE_SIZE_IN_BITS = BASE_TYPE_SIZE_IN_BYTES * 8;

	Bitset() = default;

	void initialize( s32 size_, Allocator* allocator_ = Context.stack_allocator );
	void shutdown();

	void set( s32 idx );
	void reset( s32 idx );
	void set_range( s32 idx, s32 size );
	void reset_range( s32 idx, s32 size );

	void flip( s32 idx );
	void clear();
	returning test( s32 idx ) const -> bool;

	returning find_first_unset_bit( s32 begin = 0 ) const -> s32;

private:
	Allocator* allocator;
	s32 size_in_bits = -1;
	s32 size_in_base_types = -1;
	TBaseType* data = nullptr;
};


template <typename TBaseType>
void Bitset<TBaseType>::initialize( s32 size_, Allocator* allocator_ )
{
	con_assert( size_ > 0 );
	con_assert( allocator_ != nullptr );
	con_assert( data == nullptr );

	allocator = allocator_;
	size_in_bits = size_;
	size_in_base_types = size_in_bits / BASE_TYPE_SIZE_IN_BITS + 1;

	data = reinterpret_cast<TBaseType*>( allocator->allocate( BASE_TYPE_SIZE_IN_BYTES * size_in_base_types ) );

	memset( data, 0, BASE_TYPE_SIZE_IN_BYTES * size_in_base_types );
}

template <typename TBaseType>
void Bitset<TBaseType>::shutdown()
{
	if ( data == nullptr ) {
		return;
	}
	
	allocator->free( data, BASE_TYPE_SIZE_IN_BYTES * size_in_base_types );
	data = nullptr;
	size_in_bits = -1;
	size_in_base_types = -1;
	allocator = nullptr;
}

template <typename TBaseType>
void Bitset<TBaseType>::set( s32 idx )
{
	con_assert( idx < size_in_bits );
	data[idx / BASE_TYPE_SIZE_IN_BITS] |= ( 1 << ( idx%BASE_TYPE_SIZE_IN_BITS ) );
}

template <typename TBaseType>
void Bitset<TBaseType>::reset( s32 idx )
{
	con_assert( idx < size_in_bits );
	data[idx / BASE_TYPE_SIZE_IN_BITS] &= ~( 1 << ( idx%BASE_TYPE_SIZE_IN_BITS ) );
}

template <typename TBaseType>
void Bitset<TBaseType>::set_range( s32 idx, s32 size )
{
	// @Performance: we probably can do better than that (setting every byte instead of every individual
	// bit), but we don't need such microoptimalizations

	con_assert( idx + size < size_in_bits );
	for ( s32 i = idx; i < idx + size; ++i ) {
		data[i / BASE_TYPE_SIZE_IN_BITS] |= ( 1 << ( i%BASE_TYPE_SIZE_IN_BITS ) );
	}
}

template <typename TBaseType>
void Bitset<TBaseType>::reset_range( s32 idx, s32 size )
{
	con_assert( idx + size < size_in_bits );
	for ( s32 i = idx; i < idx + size; ++i ) {
		data[i / BASE_TYPE_SIZE_IN_BITS] &= ~( 1 << ( i%BASE_TYPE_SIZE_IN_BITS ) );
	}
}

template <typename TBaseType>
void Bitset<TBaseType>::flip( s32 idx )
{
	con_assert( idx < size_in_bits );
	data[idx/ BASE_TYPE_SIZE_IN_BITS] ^= ( 1 << idx%BASE_TYPE_SIZE_IN_BITS );
}

template <typename TBaseType>
void Bitset<TBaseType>::clear()
{
	memset( data, 0, BASE_TYPE_SIZE_IN_BYTES * size_in_base_types );
}

template <typename TBaseType>
auto Bitset<TBaseType>::test( s32 idx ) const -> bool
{
	con_assert( idx < size_in_bits );
	return ( data[idx / BASE_TYPE_SIZE_IN_BITS] & ( 1 << ( idx % BASE_TYPE_SIZE_IN_BITS ) ) );
}

template<typename TBaseType>
returning Bitset<TBaseType>::find_first_unset_bit( s32 begin ) const -> s32
{
	constant begin_in_base_types = begin / BASE_TYPE_SIZE_IN_BITS;

	for ( s32 i = begin_in_base_types; i < size_in_base_types; ++i ) {
		// If all bits are set, continue.
		if ( data[i] == std::numeric_limits<TBaseType>::max() ) {
			continue;
		}

		for ( s32 bit = i*BASE_TYPE_SIZE_IN_BITS; bit < ( i+1 )*BASE_TYPE_SIZE_IN_BITS; ++bit ) {
			if ( test( bit ) == false ) {
				return bit;
			}
		}
	}

	return -1;
}
}
