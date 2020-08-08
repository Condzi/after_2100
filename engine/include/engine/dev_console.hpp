#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>
#include <engine/render_info.hpp>
#include <engine/font.hpp>

namespace con
{

// The console is located right above the screen. When we open it, it moves down. When we close it,
// it moves up.
// We also use this to scroll the text. We always render entire buffer, but when scrolling we move it up and down.
class Dev_Console final
{
	// It has to have access to the Render_Infos.
	friend class Renderer;

public:

	/*
		We initialize it in two steps.
		* First, set up the buffer to save the messages. Do this as early as possible to not miss the initialization logs. (probably still miss some?)
		* Second, initialize the visuals. Text rendering, backgrounds because we need OpenGL context for that.
	*/
	void initlialize_buffers();
	// Break the initialization_logs_buffer strings here into multiple lines.
	void initialize_graphics( Font* font_ );

	void shutdown();

	// @Important: when we resize the window we need to update our visuals
	// and chars_per_line, etc. because we calculate them using screen size.
	// void update_metrics()

	// @ToDo: add some cool animation. Not so slow, just don't let it be an instant
	// pop-up. 
	void show();
	void hide();

	// We break the data into lines which length depends on screen width.
	void print( CString data );

	// Called by the Application. Executed only if `graphics_need_update` and `visible` is true.
	// We do it this way to not call this function in every print( ... ) call.
	// We also update here the opening/closing animation.
	void update();

private:
	compile_constant message_buffer_size      = CON_DEV_CONSOLE_BUFFER_SIZE;
	compile_constant init_logs_buffer_size    = CON_DEV_CONSOLE_INIT_BUFFER_SIZE;
	compile_constant occupied_height          = CON_DEV_CONSOLE_OCCUPIED_HEIGHT;

	// In pixels. We calculate it depending on the occupied_height * win_height.
	s32 max_openess   = -1;
	// First queried from Config_File (local.variables). If we update the window
	// resolution, then we get them from created (resized) window.
	// I've decided to save it here to be explicit about what window size we're using.s
	s32 window_width  = -1;
	s32 window_height = -1;

	// Again, depending on the screen size & font metrics.
	s32 line_length = -1;

	// The console is fully closed by default.
	struct
	{
		bool initialization_stage : 1 = true; // are we using init_logs_buffer?
	
		bool update_openees : 1   = false;
		bool update_closeness : 1 = false;
		bool fully_open : 1       = false;
		bool fully_closed : 1     = true;
		bool update_text : 1      = false;
		bool update_matrices : 1  = false;
	} flags;

	// First ones are the oldest.
	// We want to move the data to the beginning and put the new data at the end (where the space have been freed by moving)
	Array<CString> message_buffer;
	// When we're at the initialization stage of the game, we don't have information
	// about the line length. Therefore, we have to postpone that. We correctly
	// break the data into lines in the `initialize_graphics` function.
	// We also free it there.
	Array<char> init_logs_buffer;
	// Next free space to log.
	s32 initialization_logs_buffer_idx = 0;

	struct
	{
		Render_Info text;
		Render_Info background;
	} render_infos;

	struct
	{
		mat4 text;
		mat4 background;
	} matrices;

	struct
	{
		v2 text;
		v2 background;
	} positions;

};
}