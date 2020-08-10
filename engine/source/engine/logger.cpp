#include <engine/logger.hpp>
#include <engine/dev_console.hpp>

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

void Logger::push_indent()
{
	current_indent += 4;
}

void Logger::pop_indent()
{
	current_indent -= 4;
	con_assert( current_indent >= 0 );
}

void Logger::log( CString message )
{
	assert( next_free_slot + message.size + current_indent < end );

	// We use spaces instad of tabs for easier handling.
	memset( next_free_slot, ' ', current_indent );
	next_free_slot += current_indent;

	memcpy( next_free_slot, message.data, message.size );

	Context.dev_console->print( CString{ next_free_slot - current_indent, message.size + current_indent } );

	next_free_slot += message.size;

	// We add newline here because the Dev_Console doesn't want \n.
	*next_free_slot = '\n';
	++next_free_slot;
}

void Logger::log_no_indent( CString message )
{
	assert( next_free_slot + message.size < end );

	memcpy( next_free_slot, message.data, message.size );
	Context.dev_console->print( CString{next_free_slot, message.size } );

	next_free_slot += message.size;
	// We add newline here because the Dev_Console doesn't want \n.
	*next_free_slot = '\n';
	++next_free_slot;
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