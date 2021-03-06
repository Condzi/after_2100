#include <engine/dev_console.hpp>

#include <engine/window.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/logger.hpp>
#include <engine/renderer.hpp>	
#include <engine/input.hpp>

#include <glm/gtx/transform.hpp>

namespace con
{
void Dev_Console::initialize_buffers()
{
	messages_buffer.initialize( messages_buffer_size, Context.default_allocator );
	lines_buffer   .initialize( lines_buffer_size,    Context.default_allocator );

	memset( messages_buffer.data(), 0, sizeof( CString ) * messages_buffer_size );
	memset( lines_buffer.data(),    0, sizeof( CString ) * lines_buffer_size );
}

void Dev_Console::initialize_graphics()
{
	con_push_indent();
	defer{ con_pop_indent(); };

	font = &Context.prepared_resources->get_font( "dev_console"_hcs );

	//
	// First, we get some constants which then we use to calculate our fields.
	//
	constant space_width       = font->get_character_info( L' ', Text_Size::Developer_Console ).advance;
	constant line_spacing_info = font->get_line_spacing( Text_Size::Developer_Console );
	constant line_height       = static_cast<s32>( line_spacing_info.ascent - line_spacing_info.descent );

	constant window_width  = Context.window->width();
	constant window_height = Context.window->height();


	height = static_cast<s32>( occupied_height * window_height );
	// Make height to be multiples of line_height.
	height = line_height * ( height / line_height ) + 1;

	lines_count  = height / line_height - 1;
	// We might have veeeeery tall screens in the future.
	if ( lines_count >= lines_buffer_size ){
		lines_count = lines_buffer_size - 1;
	}
	lines_length = ( window_width / space_width ) - 1;

	current_top_line = lines_buffer_size - lines_count;

	con_log( "Dev_Console. lines_count = %, lines_length = %, height = %, current_top_line = %.", lines_count, lines_length, height, current_top_line );

	//
	// Initialize the graphics
	//
	auto& text_ri = graphic_elements.text.render_info;

	text_ri = construct_text( L"foo", *font,  Text_Size::Developer_Console, -1 ).render_info;
	text_ri.shader        = Context.prepared_resources->get_shader( "text"_hcs );
	text_ri.texture.id    = font->get_texture( Text_Size::Developer_Console );
	text_ri.drawing_group = Drawing_Group::Dev_Console;
	text_ri.model_mat     = mat4{ 1.0f };

	auto& bg_pos = graphic_elements.background.position;
	bg_pos = v2( window_width / 2, height / 2 );
	auto& bg_ri = graphic_elements.background.render_info;

	bg_ri = construct_rectangle( window_width, height );
	bg_ri.shader        = Context.prepared_resources->get_shader( "geometry_tinted"_hcs );
	bg_ri.model_mat     = glm::translate( mat4{ 1.0f }, v3{ bg_pos.x, bg_pos.y, 0.0f } );
	bg_ri.drawing_group = Drawing_Group::Dev_Console;


	text_ri.drawing_layer = 1;
	bg_ri.drawing_layer   = 0;

	text_ri.tint = Tint{ 255, 255, 255, 255 };
	bg_ri  .tint = Tint{ 16, 120, 149,  220 };

	flags.graphics_initialized = true;

	//
	// Now we can parse the messages from initialization.
	// We do it by simply copying the messages to temporary buffer
	// and printing them again.
	//

	Array<CString> temp_messages;
	temp_messages.initialize( messages_buffer_size, Context.temporary_allocator );
	memcpy( temp_messages.data(), messages_buffer.data(), messages_buffer_size * sizeof( CString ) );

	memset( messages_buffer.data(), 0, messages_buffer_size * sizeof( CString ) );

	for ( s32 i = 0; i < messages_buffer_size; ++i ){
		// We probably copied a few empty messages, too.
		if ( temp_messages[i].length <= 0 ){
			continue;
		}

		this->print( temp_messages[i] );
	}

}

void Dev_Console::shutdown()
{
	messages_buffer.shutdown();
	lines_buffer.shutdown();
}

void Dev_Console::toggle()
{
	con_assert( flags.graphics_initialized == true );

	flags.is_open = !flags.is_open;
	flags.update_text_content = true;
}

void Dev_Console::update( f32 dt )
{
	// Use the dt for the animation, later.
	unused( dt );

	if ( Context.input->is_key_pressed( "dev_console"_hcs ) ){
		toggle();
	}

	if ( !flags.is_open ){
		return;
	}

	auto& ta = reinterpret_cast<Temporary_Allocator&>( *Context.temporary_allocator );

	con_push_indent();
	defer{ con_pop_indent(); };
	
	if ( flags.update_text_content ) {
		constant ta_mark = ta.get_mark();
		defer{ ta.set_mark( ta_mark ); };

		// Construct wchar_t buffer to display.
		// It's actually reserved memory size. The real size might be different.
		constant buffer_size  = lines_count * lines_length;
		wchar_t* const buffer = ta.allocate<wchar_t>( buffer_size );
		// It's also the real size of the buffer
		s32 buffer_idx = 0;

		for ( s32 i = current_top_line; i < current_top_line + lines_count; ++i ){
			constant& current_line = lines_buffer[i];

			if ( current_line.length <= 0 ){
				continue;
			}

			constant utf8_str = cstring_to_utf8_string( lines_buffer[i] );
			memcpy( buffer + buffer_idx, utf8_str.data, utf8_str.length * sizeof( wchar_t ) );

			buffer_idx += utf8_str.length;
			// If we have multiline message we're having newline sometimes.
			// Therefore, add newline only if we haven't already.
			if ( buffer[buffer_idx-1] != L'\n' ){
				buffer[buffer_idx] = L'\n';
				++buffer_idx;
			}
		}

		UTF8_String const final_string_to_display{ buffer, buffer_idx };

		auto& text_ri = graphic_elements.text.render_info;

		constant old_mat     = text_ri.model_mat;
		constant old_texture = text_ri.texture;
		constant old_shader  = text_ri.shader;
		constant old_tint    = text_ri.tint;
		constant old_group   = text_ri.drawing_group;
		constant old_layer   = text_ri.drawing_layer;

		constant[new_ri, size] = construct_text( final_string_to_display, *font, Text_Size::Developer_Console, -1 );

		con_assert( size.x < Context.window->width() );
		con_assert( size.y <= height );

		text_ri               = new_ri;
		text_ri.model_mat     = old_mat;
		text_ri.texture       = old_texture;
		text_ri.shader        = old_shader;
		text_ri.tint          = old_tint;
		text_ri.drawing_group = old_group;
		text_ri.drawing_layer = old_layer;

		flags.update_text_content = false;
	}
	

	if ( flags.update_matrices ){
		constant& text_pos = graphic_elements.text.position;
		auto& text_mat = graphic_elements.text.render_info.model_mat;

		text_mat = glm::translate( mat4{ 1.0f }, v3( text_pos.x, text_pos.y, 0.0f ) );

		constant& bg_pos = graphic_elements.background.position;
		auto& bg_mat = graphic_elements.background.render_info.model_mat;

		bg_mat = glm::translate( mat4{ 1.0f }, v3{ bg_pos.x, bg_pos.y, 0.0f } );

		flags.update_matrices = false;
	}
}


void Dev_Console::print( CString message )
{
	flags.update_text_content = true;

	// Move the messages up, discard the last (oldest) one.
	memmove( messages_buffer.data(), messages_buffer.data() + 1, ( messages_buffer_size - 1 ) * sizeof( CString ) );

	messages_buffer[messages_buffer_size - 1] = message;

	if ( !flags.graphics_initialized ){
		return;
	}

	Array<CString> temp_lines_buffer;
	// @ToDo: move the magic number of temp lines to a macro.
	temp_lines_buffer.initialize( 32, Context.temporary_allocator );

	//
	// Break into multiple lines if needed.
	//
	s32 current_line_begin      = 0;
	s32 last_space_position     = -1;
	s32 current_character_count = 0;
	// It's also the amount of lines we've added.
	s32 current_line_idx        = 0;

	// Helper lambda for adding lines.
	constant add_line = [&]( s32 const start, s32 const length ){
		temp_lines_buffer[current_line_idx] = CString{ message.data + start, length };

		++current_line_idx;
		con_assert( current_line_idx < 32 );
	};

	for ( s32 i = 0; i < message.length; ++i ){
		++current_character_count;

		if ( message.data[i] == ' ' ){
			last_space_position = i;
		} else if ( message.data[i] == '\n' ||
					( current_character_count > lines_length ) ){
			// In case there is no space character, break the word.
			// If there is '\n', break there.
			if ( last_space_position == -1 || message.data[i] == '\n' ){
				last_space_position = i;
			}

			constant current_line_length = last_space_position - current_line_begin + 1;

			add_line( current_line_begin, current_line_length );

			current_character_count = i - last_space_position - 1;
			current_line_begin      = last_space_position + 1;
			last_space_position     = -1;

			// This means that there is only one line remaining.
			if ( i == message.length - 1 ){
				// Adding the last line.
				add_line( current_line_begin, current_character_count );
				break;
			}
		}

		// @Performance: multiline messages are rather seldom. We use them to print
		// check failures. Maybe check if given message has any \n or is longer than
		// line_length, then call print_multiline?

		// It means that we're at the end and there was no \n.
		// (so it's just one line message)
		if ( current_character_count == message.length - 1 ){
			add_line( 0, message.length );
		}
	}

	free_lines_in_the_buffer_count -= current_line_idx;
	if ( free_lines_in_the_buffer_count < 0 ){
		free_lines_in_the_buffer_count = 0;
	}

	// Move the current lines "up" (we're getting rid of some lines too)
	memmove( lines_buffer.data(), lines_buffer.data() + current_line_idx, ( lines_buffer_size - current_line_idx ) * sizeof( CString ) );
	// Copy the new lines.
	memcpy( lines_buffer.data() + lines_buffer_size - current_line_idx, temp_lines_buffer.data(), current_line_idx * sizeof( CString ) );
}

void Dev_Console::scroll_up()
{
	if( flags.is_open &&
		current_top_line - 1 >= free_lines_in_the_buffer_count
		) {
		current_top_line--;

		flags.update_text_content = true;
	}
}

void Dev_Console::scroll_down()
{
	if( flags.is_open &&
		current_top_line + 1 <= lines_buffer_size - lines_count ) {
		current_top_line++;

		flags.update_text_content = true;
	}
}

returning Dev_Console::is_open() const -> bool
{
	return flags.is_open;
}
}
