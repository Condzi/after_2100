#include <engine/allocator.hpp>

#include <cstdlib>

namespace con
{
pure Default_Allocator::initialize()
{
    conassert( false );
}

pure Default_Allocator::shutdown()
{
    conassert( false );
}
pure Default_Allocator::free( byte* location, s32 size )
{
    unused( size );
    std::free( location );
}

returning Default_Allocator::allocate( s32 size ) -> byte* 
{
    byte* memory{ reinterpret_cast<byte*>( std::malloc( size ) ) };
    conassert( memory != nullptr );
    return memory;
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

    return memory;
}
}