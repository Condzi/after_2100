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
}

void Logger::shutdown()
{
	Context.default_allocator->free( reinterpret_cast<byte*>( begin ), buffer_size );
}

void Logger::log( CString message, s32 indent )
{
	assert( next_free_slot + message.size + indent < end );

	for ( s32 i = 0; i < indent; ++i ) {
		*next_free_slot = '\t';
		++next_free_slot;
	}

	memcpy( next_free_slot, message.data, message.size );
	next_free_slot += message.size;
}

void Logger::reset_buffer()
{
	next_free_slot = begin;
}

returning Logger::get_buffer() const -> CString
{
	constant messages_size = static_cast<s32>( next_free_slot - begin );
	if ( messages_size == 0 ) {
		return { nullptr, 0 };
	}

	// We're adding \0 for C's puts at the end, so we need an extra space.
	assert( messages_size + 1 < buffer_size );
	*next_free_slot = '\0';
	return { begin, messages_size };
}
}