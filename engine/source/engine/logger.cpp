#include <engine/logger.hpp>

namespace con
{
pure Logger::set_instance( Logger* instance_ )
{
	instance = instance_;
}

returning Logger::get_instance() -> Logger&
{
	return *instance;
}

pure Logger::initialize()
{
	auto* memory = reinterpret_cast<char*>( Context.default_allocator->allocate( CON_LOGGER_BUFFER_RESERVED_MEMORY ) );

	buffer = { memory, CON_LOGGER_BUFFER_RESERVED_MEMORY };
	// @Robustness: this const_cast is ugly -- consider adding something like a 
	// Byte_String / Binary_String?
	next_free_slot = const_cast<char*>( buffer.data );
}

pure Logger::shutdown()
{
	Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( buffer.data ) ), buffer.size );
}

pure Logger::log( CString message, s32 indent )
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

pure Logger::reset_buffer()
{
	next_free_slot = const_cast<char*>( buffer.data );
}

returning Logger::get_buffer() -> CString
{
	return { buffer.data, static_cast<s32>( next_free_slot - buffer.data ) };
}
}