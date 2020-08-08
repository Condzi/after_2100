#include <engine/dev_console.hpp>

#include <engine/config_file.hpp>
#include <engine/formatting.hpp>

namespace con
{
void Dev_Console::initlialize_buffers()
{
	message_buffer.initialize( message_buffer_size, Context.default_allocator );
	init_logs_buffer.initialize( init_logs_buffer_size, Context.default_allocator );
}

void Dev_Console::initialize_graphics( Font* font_ )
{
	
	init_logs_buffer.shutdown();
	flags.initialization_stage = false;
}

void Dev_Console::shutdown()
{
	message_buffer.shutdown();
}

void Dev_Console::print( CString data )
{
	// Maybe do some special function, like print_in_app_init()?
	if ( flags.initialization_stage ) {
		con_assert( initialization_logs_buffer_idx + data.size < init_logs_buffer_size );

		memcpy( init_logs_buffer.data() + initialization_logs_buffer_idx, data.data, data.size );

		initialization_logs_buffer_idx += data.size;

	} else {

	}

	flags.update_text = true;
}
}