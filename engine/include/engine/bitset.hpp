#pragma once

#include <engine/assert.hpp>
#include <engine/typedefs.hpp>


namespace con
{
class Allocator;

// We use con_cassert because it's a core functionality. It's being used in allocators.
class Bitset final
{
public:
	compile_constant base_type_size_in_bytes = static_cast<s32>( sizeof( u64 ) );
	compile_constant base_type_size_in_bits  = base_type_size_in_bytes * 8;

	// size_ is the size in bits.
	void initialize( s32 size_, Allocator* allocator_ );
	void shutdown();

	void set(   s32 idx );
	void reset( s32 idx );

	void set_range(   s32 idx, s32 size );
	void reset_range( s32 idx, s32 size );

	void flip( s32 idx );
	void clear();

	returning test( s32 idx ) const -> bool;

	returning find_first_unset_bit( s32 begin = 0 ) const -> Find_Result;
	returning count_set_bits() const -> s32;

private:
	Allocator* allocator   = nullptr;
	u64* data              = nullptr;
	s32 size_in_bits       = -1;
	s32 size_in_base_types = -1;

	// @Robustness: consider adding `data_size_in_bytes` to the Bitset.
	// We often use `base_type_size_in_bytes * size_in_base_types` combo, which
	// is equal to size of `data` in bytes. -- condzi, 12.08.2020
};
}