#pragma once

#include <engine/context.hpp>
#include <engine/assert.hpp>
#include <engine/macro_config.hpp>
#include <engine/typedefs.hpp>
#include <engine/bitset.hpp>

namespace con
{
struct Allocator
{
	returning virtual allocate( s32 size ) -> byte* = 0;
	pure virtual free( byte* location, s32 size ) = 0;
};

// @ToDo: Make 4 byte sized blocks and keep track of free memory using
// our bitset class. (position in bitset*4 = position in memory).
// For now just use malloc/free.
struct Default_Allocator final :
	Allocator
{
	// @ToDo:
	// Allocates CON_RESERVED_MEMORY amount of memory.
	pure initialize();
	pure shutdown();

	returning allocate( s32 size ) -> byte* override;
	// Just allocates amount*sizeof(T) bytes and does reinterpret_cast 
	template <typename T>
	returning allocate( s32 amount = 1 ) -> T*;
	pure free( byte* location, s32 size ) override;

private:
	using Used_Bytes_Bitset = Bitset<CON_RESERVED_MEMORY>;

	byte* begin = nullptr;
	compile_constant reserved_size = CON_RESERVED_MEMORY;
	Used_Bytes_Bitset* used_bytes = nullptr; // allocating on stack causes stack overflow
};

// Reserves and manages memory from Default_Allocator
struct Temporary_Allocator final :
	Allocator
{
	pure initialize( s32 reserved = CON_TEMPORARY_STORAGE_RESERVED_MEMORY );

	returning allocate( s32 size ) -> byte* override;
	template <typename T>
	returning allocate( s32 amount = 1 ) -> T*;
	// Does nothing.
	pure free( byte* location, s32 size ) override;

	pure reset();

	pure set_mark( s32 new_mark );
	returning get_mark() -> s32;
	returning get_highest_mark() -> s32;

private:
	byte* memory;
	s32 size;
	s32 mark;
	s32 highest_mark;
};


//
// Definitions
//
template <typename T>
returning Default_Allocator::allocate( s32 amount ) -> T*
{
	return reinterpret_cast<T*>( allocate( sizeof( T ) * amount ) );
}

template <typename T>
returning Temporary_Allocator::allocate( s32 amount ) -> T*
{
	return reinterpret_cast<T*>( allocate( sizeof( T ) * amount ) );
}

}