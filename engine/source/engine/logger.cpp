#include <engine/logger.hpp>

// We don't use con_assert because con_assert uses logger too.
#include <cassert>

namespace con
{
void Logger::initialize()
{
	begin = reinterpret_cast<char*>( Context.default_allocator->allocate( buffer_size ) );
	end = begin + buffer_size;
	next_free_slot = begin;

	memset( begin, 0, buffer_size );
}

void Logger::shutdown()
{
	Context.default_allocator->free( reinterpret_cast<byte*>( begin ), buffer_size );
}

void Logger::log( CString message, s32 indent )
{
	assert( next_free_slot + message.size + indent < end );

	memset( next_free_slot, '\t', indent );
	memcpy( next_free_slot + indent, message.data, message.size );

	// We can do this because logger buffer is initialized with zeros.
	printf( "%s", next_free_slot );

	next_free_slot += message.size + indent;
}

returning Logger::get_buffer() const -> CString
{
	constant messages_size = static_cast<s32>( next_free_slot - begin );
	if ( messages_size == 0 ) {
		return { nullptr, 0 };
	}

	// We don't have to add \0 because buffer is initialized with zeros.

	return { begin, messages_size };
}
}