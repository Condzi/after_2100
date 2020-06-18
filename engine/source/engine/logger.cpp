#include <engine/logger.hpp>

namespace con
{
void Logger::initialize()
{
	auto* memory = reinterpret_cast<char*>( Context.default_allocator->allocate( CON_LOGGER_BUFFER_RESERVED_MEMORY ) );

	buffer = { memory, CON_LOGGER_BUFFER_RESERVED_MEMORY };
	// @Robustness: this const_cast is ugly -- consider adding something like a 
	// Byte_String / Binary_String?
	next_free_slot = const_cast<char*>( buffer.data );
}

void Logger::shutdown()
{
	Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( buffer.data ) ), buffer.size );
}

void Logger::log( CString message, s32 indent )
{
	constant end = buffer.data + buffer.size;
	con_assert( next_free_slot +  message.size + indent < end );

	for ( s32 i = 0; i < indent; ++i ) {
		*next_free_slot = '\t';
		++next_free_slot;
	}

	std::memcpy( next_free_slot, message.data, message.size );
	next_free_slot += message.size;
}

void Logger::reset_buffer()
{
	next_free_slot = const_cast<char*>( buffer.data );
}

returning Logger::get_buffer() -> CString
{
	constant messages_size = static_cast<s32>( next_free_slot - buffer.data );
	if ( messages_size == 0 ) {
		return { nullptr, 0 };
	}
	// We're adding \0 for C's puts at the end, so we need an extra space.
	release_con_assert( messages_size + 1 < buffer.size );
	*next_free_slot = '\0';
	return { buffer.data, messages_size };
}
}