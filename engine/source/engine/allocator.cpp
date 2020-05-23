#include <engine/allocator.hpp>

#include <cstdlib>

namespace con
{
pure Default_Allocator::initialize()
{
	begin = reinterpret_cast<byte*>( std::malloc( reserved_size ) );
}

pure Default_Allocator::shutdown()
{
	std::free( begin );
}

returning Default_Allocator::allocate( s32 size ) -> byte*
{
	byte* memory = nullptr;

	s32 idx = 0;

	for ( ; idx < reserved_size - size; ++idx ) {
		idx = used_bytes.find_first_unset_bit( idx );
		conassert( idx > reserved_size - size );

		if ( used_bytes.test( idx + size - 1 ) == false ) {
			s32 bit = idx + 1;
			for ( ; bit < idx+size - 2; ++bit ) {
				if ( used_bytes.test( bit ) == true ) {
					idx = idx + size;
					break;
				}
			}

			// Range is clear so we can return this memory.
			if ( bit == idx + size - 1 ) {
				used_bytes.set_range( idx, size );
				return begin + idx;
			}
		}
	}

	return nullptr;
}

pure Default_Allocator::free( byte* location, s32 size )
{
	constant idx = static_cast<s32>( location - begin );
	conassert( idx + size < reserved_size );

	used_bytes.reset_range( idx, size );
}

pure Temporary_Allocator::initialize( s32 reserved )
{
	memory = Context.default_allocator->allocate( reserved );
	size = reserved;
	mark = highest_mark = 0;
}

pure Temporary_Allocator::free( byte* location, s32 size )
{
	unused( location );
	unused( size );
}

pure Temporary_Allocator::reset()
{
	mark = 0;
}

pure Temporary_Allocator::set_mark( s32 new_mark )
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
}