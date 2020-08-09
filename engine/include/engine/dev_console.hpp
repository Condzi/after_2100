#include <engine/render_info.hpp>
#include <engine/array.hpp>
#include <engine/cstring.hpp>
#include <engine/font.hpp>

namespace con
{
class Dev_Console final
{
public:
	struct Graphic_Element final
	{
		v2          position = v2( 0,0 );
		Render_Info render_info;
	};

	// They're public because Renderer has to access them.
	struct
	{
		Graphic_Element text;
		Graphic_Element background;
	} graphic_elements;


	void initialize_buffers();
	// Call after creating the window and loading the dev_console font.
	void initialize_graphics();

	void shutdown();

	void toggle();

	// Update the animation and the text, if needed.
	void update( f32 dt );
	// @Resizing
	// void update_to_new_resolution();
	
	// When logging you have to make sure that the message will not be 
	// deleted! We aren't copying it.
	void print( CString message );

	returning is_open() const -> bool;

private:
	compile_constant messages_buffer_size = CON_DEV_CONSOLE_MESSAGES_BUFFER_SIZE;
	compile_constant lines_buffer_size    = CON_DEV_CONSOLE_LINES_BUFFER_SIZE;
	compile_constant occupied_height      = CON_DEV_CONSOLE_OCCUPIED_HEIGHT;

	// Logger owns this strings. If you want to print, you should own it.
	Array<CString> messages_buffer;
	// Lines are just split messages. Sometimes messages may be longer than
	// one line, so we have to know that.
	// 
	// Newest lines / messages are at the end.
	Array<CString> lines_buffer;

	// Both are depending on the screen width and height.
	s32 lines_count  = -1;
	s32 lines_length = -1;
	
	// The line that currently is on top / the first displaying.
	// By default it should be equal to:
	//
	//     lines_buffer.size - lines_count
	s32 current_top_line = -1;

	// In pixels. Height of the background. 
	s32 height = -1;

	Font* font = nullptr;

	struct
	{
		bool graphics_initialized : 1 = false;
		bool is_open : 1              = false;
		bool update_matrices : 1      = false;
		bool update_text_content : 1  = false;
	} flags;
};
}