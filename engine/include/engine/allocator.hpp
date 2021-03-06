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
	void virtual      free( byte* location, s32 size ) = 0;
};

struct C_Allocator final :
	Allocator
{
	returning allocate( s32 size ) -> byte* override;
	void      free( byte* location, s32 size ) override;
};

// @ToDo: Make 4 byte sized blocks and keep track of free memory using
// our bitset class. (position in bitset*4 = position in memory).
// For now just use malloc/free.
struct Default_Allocator final :
	Allocator
{
	void initialize();
	void shutdown();

	returning allocate( s32 size ) -> byte* override;
	// Just allocates amount*sizeof(T) bytes and does reinterpret_cast 
	template <typename T>
	returning allocate( s32 amount = 1 ) -> T*;
	void      free( byte* location, s32 size ) override;

private:
	compile_constant reserved_size = CON_RESERVED_MEMORY;

	byte*  begin = nullptr;
	Bitset used_bytes;
};

// Reserves and manages memory from Default_Allocator
struct Temporary_Allocator final :
	Allocator
{
	void initialize();

	returning allocate( s32 size ) -> byte* override;
	template <typename T>
	returning allocate( s32 amount = 1 ) -> T*;
	// Does nothing.
	void      free( byte* location, s32 size ) override;

	void reset();

	void      set_mark( s32 new_mark );
	returning get_mark() -> s32;
	returning get_highest_mark() -> s32;

private:
	compile_constant reserved_size = CON_TEMPORARY_STORAGE_RESERVED_MEMORY;

	byte* memory     = nullptr;
	s32 mark         = 0;
	s32 highest_mark = 0;
};

// Allocates stuff on stack.
struct Stack_Allocator final :
	Allocator
{
	void initialize();
	void shutdown();

	returning allocate( s32 size ) -> byte* override;
	void      free( byte* location, s32 size ) override;

private:
	compile_constant reserved_size = CON_STACK_RESERVED_MEMORY;

	byte buffer[reserved_size] ={ 0 };
	Bitset used_bytes;
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