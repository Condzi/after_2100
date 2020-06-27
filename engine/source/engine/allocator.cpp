#include <engine/allocator.hpp>

#include <cstdlib>

namespace con
{
void Default_Allocator::initialize()
{
	begin = Context.c_allocator->allocate( reserved_size );
	used_bytes = reinterpret_cast<Used_Bytes_Bitset*>( Context.c_allocator->allocate( sizeof( Used_Bytes_Bitset )  ) );
	new( used_bytes ) Used_Bytes_Bitset{};
}

void Default_Allocator::shutdown()
{
	Context.c_allocator->free( begin, reserved_size );
	Context.c_allocator->free( reinterpret_cast<byte*>( used_bytes ), sizeof( Used_Bytes_Bitset ) );
}

returning Default_Allocator::allocate( s32 size ) -> byte*
{
	s32 idx = 0;

	for ( ; idx < reserved_size - size; ++idx ) {
		idx = used_bytes->find_first_unset_bit( idx );
		con_assert( idx < reserved_size - size );

		if ( used_bytes->test( idx + size - 1 ) == false ) {
			s32 bit = idx + 1;
			for ( ; bit < idx+size - 1; ++bit ) {
				if ( used_bytes->test( bit ) == true ) {
					idx = idx + bit;
					break;
				}
			}

			// Range is clear so we can return this memory.
			if ( bit == idx + size - 1 ) {
				used_bytes->set_range( idx, size );
				return begin + idx;
			}
		}
	}

	return nullptr;
}

void Default_Allocator::free( byte* location, s32 size )
{
	if ( size <= 0 ) {
		return;
	}

	constant idx = static_cast<s32>( location - begin );
	con_assert( idx + size < reserved_size );

	used_bytes->reset_range( idx, size );
}

void Temporary_Allocator::initialize( s32 reserved )
{
	memory = Context.default_allocator->allocate( reserved );
	size = reserved;
	mark = highest_mark = 0;
}

void Temporary_Allocator::free( byte* location, s32 size )
{
	unused( location );
	unused( size );
}

void Temporary_Allocator::reset()
{
	mark = 0;
}

void Temporary_Allocator::set_mark( s32 new_mark )
{
	mark = new_mark;
}

returning Temporary_Allocator::get_mark() -> s32
{
	return mark;
}

returning Temporary_Allocator::get_highest_mark() -> s32
{
	return highest_mark;
}

returning Temporary_Allocator::allocate( s32 size ) -> byte*
{
	byte* requested_memory = memory + mark;
	mark += size;

	if ( mark > highest_mark ) {
		highest_mark = mark;
	}

	return requested_memory;
}

returning C_Allocator::allocate( s32 size ) -> byte*
{
	return reinterpret_cast<byte*>( malloc( size ) );
}

void C_Allocator::free( byte* location, s32 size )
{
	unused( size );

	::free( location );
}
}