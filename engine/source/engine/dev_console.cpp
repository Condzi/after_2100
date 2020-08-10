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

	con_log( "Dev_Console. lines_count = %, lines_length = %, height = %, current_top_line = %,", lines_count, lines_length, height, current_top_line );

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
	bg_ri  .tint = Tint{ 16, 120, 149,  255 };

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
		if ( temp_messages[i].size <= 0 ){
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

			if ( current_line.size <= 0 ){
				continue;
			}

			constant utf8_str = cstring_to_utf8_string( lines_buffer[i] );
			memcpy( buffer + buffer_idx, utf8_str.data, utf8_str.size * sizeof( wchar_t ) );

			buffer_idx += utf8_str.size;
			buffer[buffer_idx] = '\n';
			++buffer_idx;
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

	constant lines = message.size / lines_length + 1;
	memmove( lines_buffer.data(), lines_buffer.data() + lines, ( lines_buffer_size - lines ) * sizeof( CString ) );

	//
	// Break into multiple lines if needed.
	//

	if ( lines == 1 ){
		lines_buffer[lines_buffer_size-1] = message;
		return;
	}

	// @Incomplete: do message braking into multiple lines.
	con_assert( false );
}

void Dev_Console::scroll_up()
{
	if( flags.is_open &&
		current_top_line - 1 >= 0 ) {
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
