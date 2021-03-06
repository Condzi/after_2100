#include <engine/allocator.hpp>

#include <cstdlib>

namespace con
{
void Default_Allocator::initialize()
{
	begin = Context.c_allocator->allocate( reserved_size );
	used_bytes.initialize( reserved_size, Context.c_allocator );
}

void Default_Allocator::shutdown()
{
	Context.c_allocator->free( begin, reserved_size );
	used_bytes.shutdown();
}

returning Default_Allocator::allocate( s32 size ) -> byte*
{
	con_assert( size > 0 );

	if ( size == 1 ) {
		constant result = used_bytes.find_first_unset_bit( 0 );
		con_assert( result.found() );

		used_bytes.set( result.idx );
		return begin + result.idx;
	}

	s32 idx = 0;

	for ( ; idx < reserved_size - size; ++idx ) {
		constant result = used_bytes.find_first_unset_bit( idx );
		con_assert( result.found() );

		idx = result.idx;
		con_assert( idx < reserved_size - size );

		if ( used_bytes.test( idx + size - 1 ) == false ) {
			s32 bit = idx + 1;
			for ( ; bit < idx + size - 1; ++bit ) {
				if ( used_bytes.test( bit ) == true ) {
					idx = idx + bit;
					break;
				}
			}

			// Range is clear so we can return this memory.
			if ( bit == idx + size - 1 ) {
				used_bytes.set_range( idx, size );
				return begin + idx;
			}
		} else {
			idx = idx + size;
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

	// Check if we're clearing the correct range. If yes, then all bits in the
	// bitset should be set to true.
	//
	// @Robustness: maybe add CON_MEMORY_FREE_CHECK or something?
#if CON_DEBUG == true
	for ( s32 i = idx; i < size; ++i ){
		con_assert( used_bytes.test( i ) == true );
	}
#endif

	used_bytes.reset_range( idx, size );
}

void Temporary_Allocator::initialize()
{
	memory = Context.default_allocator->allocate( reserved_size );
	mark   = highest_mark = 0;
}

void Temporary_Allocator::free( byte* location, s32 size_ )
{
	unused( location );
	unused( size_ );
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

	con_assert( mark < reserved_size );

	return requested_memory;
}

returning C_Allocator::allocate( s32 size ) -> byte*
{
	return static_cast<byte*>( malloc( size ) );
}

void C_Allocator::free( byte* location, s32 size )
{
	unused( size );

	::free( location );
}

void Stack_Allocator::initialize()
{
	used_bytes.initialize( reserved_size, Context.c_allocator );
}

void Stack_Allocator::shutdown()
{
	used_bytes.shutdown();
}

returning Stack_Allocator::allocate( s32 size ) -> byte*
{
	s32 idx = 0;

	for ( ; idx < reserved_size - size; ++idx ) {
		constant result = used_bytes.find_first_unset_bit( idx );
		con_assert( result.found() );

		idx = result.idx;
		con_assert( idx < reserved_size - size );

		if ( used_bytes.test( idx + size - 1 ) == false ) {
			s32 bit = idx + 1;
			for ( ; bit < idx + size - 1; ++bit ) {
				if ( used_bytes.test( bit ) == true ) {
					idx = idx + bit;
					break;
				}
			}

			// Range is clear so we can return this memory.
			if ( bit == idx + size - 1 ) {
				used_bytes.set_range( idx, size );
				return buffer + idx;
			}
		}
	}

	return nullptr;
}

void Stack_Allocator::free( byte* location, s32 size )
{
	if ( size <= 0 ) {
		return;
	}

	constant idx = static_cast<s32>( location - buffer );
	con_assert( idx + size < reserved_size );

	// @Robustness: CON_MEMORY_FREE_CHECK
#if CON_DEBUG == true
	for ( s32 i = idx; i < size; ++i ){
		con_assert( used_bytes.test( i ) == true );
	}
#endif

	used_bytes.reset_range( idx, size );
}
}