#pragma once

#include <engine/assert.hpp>
#include <engine/typedefs.hpp>
#include <engine/context.hpp>

#include <cstring>
#include <limits>

namespace con
{
// @ToDo: make them internal / file_scope?
returning count_set_bits( u16 value ) -> s32;
returning find_first_unset_bit( u16 value ) -> Find_Result;

// @Idea: don't use template at all, just `byte`? Or maybe u16?
// @Important: Use unsigned types as TBaseType!!!
template <typename TBaseType = u16>
class Bitset_Base final
{
public:
	compile_constant BASE_TYPE_SIZE_IN_BYTES = static_cast<s32>( sizeof( TBaseType ) );
	compile_constant BASE_TYPE_SIZE_IN_BITS = BASE_TYPE_SIZE_IN_BYTES * 8;

	Bitset_Base() = default;

	void initialize( s32 size_, Allocator* allocator_ = Context.stack_allocator );
	void shutdown();

	void set( s32 idx );
	void reset( s32 idx );
	void set_range( s32 idx, s32 size );
	void reset_range( s32 idx, s32 size );

	void flip( s32 idx );
	void clear();
	returning test( s32 idx ) const -> bool;

	returning find_first_unset_bit( s32 begin = 0 ) const -> Find_Result;
	returning count_set_bits() const -> s32;

private:
	Allocator* allocator = nullptr;
	s32 size_in_bits = -1;
	s32 size_in_base_types = -1;
	TBaseType* data = nullptr;
};

// Using u16 allows us to use compiler intrinsic functions.
using Bitset = Bitset_Base<u16>;

//
// Definitions
//

template <typename TBaseType>
void Bitset_Base<TBaseType>::initialize( s32 size_, Allocator* allocator_ )
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
void Bitset_Base<TBaseType>::shutdown()
{
	if ( data == nullptr ) {
		return;
	}

	allocator->free( reinterpret_cast<byte*>( data ), BASE_TYPE_SIZE_IN_BYTES * size_in_base_types );
	data = nullptr;
	size_in_bits = -1;
	size_in_base_types = -1;
	allocator = nullptr;
}

template <typename TBaseType>
void Bitset_Base<TBaseType>::set( s32 idx )
{
	con_assert( idx < size_in_bits );
	data[idx / BASE_TYPE_SIZE_IN_BITS] |= ( 1 << ( idx%BASE_TYPE_SIZE_IN_BITS ) );
}

template <typename TBaseType>
void Bitset_Base<TBaseType>::reset( s32 idx )
{
	con_assert( idx < size_in_bits );
	data[idx / BASE_TYPE_SIZE_IN_BITS] &= ~( 1 << ( idx%BASE_TYPE_SIZE_IN_BITS ) );
}

template <typename TBaseType>
void Bitset_Base<TBaseType>::set_range( s32 idx, s32 size )
{
	// @Performance: we probably can do better than that (setting every byte instead of every individual
	// bit), but we don't need such microoptimalizations right now

	con_assert( idx + size < size_in_bits );
	for ( s32 i = idx; i < idx + size; ++i ) {
		data[i / BASE_TYPE_SIZE_IN_BITS] |= ( 1 << ( i%BASE_TYPE_SIZE_IN_BITS ) );
	}
}

template <typename TBaseType>
void Bitset_Base<TBaseType>::reset_range( s32 idx, s32 size )
{
	con_assert( idx + size < size_in_bits );
	for ( s32 i = idx; i < idx + size; ++i ) {
		data[i / BASE_TYPE_SIZE_IN_BITS] &= ~( 1 << ( i%BASE_TYPE_SIZE_IN_BITS ) );
	}
}

template <typename TBaseType>
void Bitset_Base<TBaseType>::flip( s32 idx )
{
	con_assert( idx < size_in_bits );
	data[idx/ BASE_TYPE_SIZE_IN_BITS] ^= ( 1 << idx%BASE_TYPE_SIZE_IN_BITS );
}

template <typename TBaseType>
void Bitset_Base<TBaseType>::clear()
{
	memset( data, 0, BASE_TYPE_SIZE_IN_BYTES * size_in_base_types );
}

template <typename TBaseType>
auto Bitset_Base<TBaseType>::test( s32 idx ) const -> bool
{
	con_assert( idx < size_in_bits );
	// @Performance: we can use intrinsic here _bittest
	return ( data[idx / BASE_TYPE_SIZE_IN_BITS] & ( 1 << ( idx % BASE_TYPE_SIZE_IN_BITS ) ) );
}

template<typename TBaseType>
returning Bitset_Base<TBaseType>::find_first_unset_bit( s32 begin ) const -> Find_Result
{
	// @Robustness: just change it to u16 and everything will be simpler. For real,
	// we have no real reason to keep u8/u16 as default now.
	static_assert( std::is_same_v<u16, TBaseType>, "find_first_unset_bit is working only for u16" );

	// We loop until we get to k * BASE_TYPE_SIZE_IN_BITS, then we can safelly
	// use the find_first_unset_bit function.

	s32 bit = begin;
	for ( ; bit % BASE_TYPE_SIZE_IN_BITS != 0 && bit < size_in_bits; ++bit ) {
		if ( test( bit ) == false ) {
			return { .idx = bit };
		}
	}

	if ( bit > size_in_bits-1 ) {
		return { .idx = -1 };
	}

	s32 u16_idx = bit / BASE_TYPE_SIZE_IN_BITS;
	for ( ; u16_idx < size_in_base_types; ++u16_idx ) {
		constant result = con::find_first_unset_bit( data[u16_idx] );

		if ( result.found() ) {
			// We may have exceed declared size in bits (but we never do that with size_in_base_types)
			// so we have to check for that.
			constant bit_to_return = u16_idx * BASE_TYPE_SIZE_IN_BITS + result.idx;
			if ( bit_to_return >= size_in_bits ) {
				return { .idx = -1 };
			} else {
				return { .idx = bit_to_return };
			}
		}
	}

	return { .idx = -1 };
}

template <typename TBaseType>
returning Bitset_Base<TBaseType>::count_set_bits() const -> s32
{
	static_assert( std::is_same_v<u16, TBaseType>, "count_set_bits is working only for u16" );

	s32 sum = 0;
	for ( s32 i = 0; i < size_in_base_types; ++i ) {
		sum += con::count_set_bits( data[i] );
	}

	return sum;
}
}
