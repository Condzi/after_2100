#pragma once

#include <engine/typedefs.hpp>
#include <engine/cstring.hpp>

struct GLFWwindow;

namespace con
{
struct Window final
{
public:
	void initialize();
	void shutdown();

	void close();

	void set_title( CString title );
	void set_clear_color( u8 r, u8 g, u8 b );
	void clear();
	void display();
	
	returning should_close() const -> bool;
	returning width() const  -> s32;
	returning height() const -> s32;

	// @ToDo: we'll eventually need this stuff if we wanna have a serious game. Don't forget the glViewport calls! And update the renderer window size too!
	// void change_resloution( s32 width, s32 height );
	// void switch_to_fullscreen();
	// void switch_from_fullscreen( s32 width, s32 height );

	returning get_internal_handle() -> GLFWwindow*;

private:
	GLFWwindow* handle;
};
}