#include <engine/assert.hpp>
#include <engine/window.hpp>
#include <engine/context.hpp>
#include <engine/logger.hpp>
#include <engine/config_file.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace con
{
void Window::initialize()
{
	//
	// Getting the display values.
	//
	con_log_indented( 1, "Gathering the window config values..." );
	auto& cfg = *Context.config_file;

	constant gl_debug   = cstring_to_bool( cfg.get_value( "debug"_hcs, "gl_debug"_hcs ) );
	constant vsync      = cstring_to_bool( cfg.get_value( "display"_hcs, "vsync"_hcs ) );
	constant fullscreen = cstring_to_bool( cfg.get_value( "display"_hcs, "fullscreen"_hcs ) );
	constant frame_cap  = cstring_to_s32( cfg.get_value( "display"_hcs, "frame_cap"_hcs ) );;
	s32 gl_major = -1, gl_minor = -1;
	s32 win_width = -1, win_height = -1;

	CString gl_context_version_cstring;
	if ( gl_debug ) {
		gl_context_version_cstring = cfg.get_value( "display"_hcs, "debug_context_version"_hcs );
	} else {
		gl_context_version_cstring = cfg.get_value( "display"_hcs, "release_context_version"_hcs );
	}

	sscan( "%.%", gl_context_version_cstring, gl_major, gl_minor );
	constant window_size_cstring = cfg.get_value( "display"_hcs, "window_size"_hcs );
	sscan( "% %", window_size_cstring, win_width, win_height );

	//
	// Initializing GLFW and setting up the window
	//
	con_log_indented( 1, "Initializing GLFW and setting up the window..." );
	glfwSetErrorCallback( con::priv::on_glfw_error );

	release_con_assert( glfwInit() == GLFW_TRUE );


	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint( GLFW_FOCUSED, GL_TRUE );
	glfwWindowHint( GLFW_CENTER_CURSOR, GL_FALSE );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, gl_major );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, gl_minor );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GL_TRUE );

	if ( !vsync ) {
		//	glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );
		if ( frame_cap > 0 ) {
			con_log_indented( 2, "Warning: frame cap is not implemented yet!!" );
		}
	}

	if ( gl_debug ) {
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
		// When we're not debbuging we're using Gl 3.0 which does not need this.
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	} else {
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE );
	}


	if ( fullscreen ) {
		GLFWmonitor* const monitor = glfwGetPrimaryMonitor();
		con_assert( monitor != nullptr );
		GLFWvidmode const* const monitor_video_mode = glfwGetVideoMode( monitor );
		con_assert( monitor_video_mode != nullptr );
		// Does char = utf-8? ughhh I hope so
		con_log_indented( 1, "Creating fullscreen window on monitor \"%\" (% x %, %Hz), window size is % x %.", cstring_from_cstr( glfwGetMonitorName( monitor ) ), monitor_video_mode->width, monitor_video_mode->height, monitor_video_mode->refreshRate, win_width, win_height );

		// @ToDo: irl use monitor_video_mode to find the best resoulution. Maybe use the gatherded
		// resolution if resoulution in cfg file is really tiny? Like "if width < monitor_width/4 or height < monitor_height < 4" or something like that? 
		// But this should happen only when settings file isn't generated / on the first launch, I belive
		// becuase maybe someone wants to play in 800x600 on a 4k monitor?!
		handle = glfwCreateWindow( win_width, win_height, CON_WINDOW_TITLE, monitor, nullptr );
		release_con_assert( handle != nullptr );
		glfwMakeContextCurrent( handle );
	} else {
		con_log_indented( 1, "Creating windowed window, size % x %.", win_width, win_height );
		// pass monitor if you want fullscreen
		handle = glfwCreateWindow( win_width, win_height, CON_WINDOW_TITLE, nullptr, nullptr );
		release_con_assert( handle != nullptr );
		glfwMakeContextCurrent( handle );
	}

	//
	// Initializing OpenGL
	//
	con_log_indented( 1, "Initializing OpenGL..." );
	release_con_assert( gladLoadGLLoader( reinterpret_cast<GLADloadproc>( glfwGetProcAddress ) ) != 0 );
	con_log_indented( 2, "Got OpenGL: %.%; requested: %.%.", GLVersion.major, GLVersion.minor, gl_major, gl_minor );


	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &Context.machine_info.max_texture_size );
	con_log_indented( 2, "Max texture size: %x%", Context.machine_info.max_texture_size, Context.machine_info.max_texture_size );

	if ( vsync ) {
		glfwSwapInterval( 1 );
	} else {
		glfwSwapInterval( 0 );
	}

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glViewport( 0, 0, win_width, win_height );

	if ( !gl_debug ) {
		con_log_indented( 2, "gl debug hasn't been requested." );

		return;
	}

	con_log_indented( 2, "Setting up gl debug stuff..." );

	release_con_assert( glDebugMessageCallback != nullptr );

	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );

	glDebugMessageCallback( []( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param ) {
		unused( user_param );
		unused( severity );
		unused( source );

		// I just couldn't get glDebugMessage to work.
		if ( id == 131185 ) {
			return;
		}


		CString const type_str = [&type] {
			switch ( type ) {
			case GL_DEBUG_TYPE_PERFORMANCE:
			return "PERFORMANCE"_cs;

			case GL_DEBUG_TYPE_OTHER:
			return "OTHER"_cs;

			case GL_DEBUG_TYPE_MARKER:
			return "MARKER"_cs;

			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "DEPRECATED"_cs;

			case GL_DEBUG_TYPE_PORTABILITY:
			return "PORTABILITY"_cs;

			case GL_DEBUG_TYPE_ERROR:
			return "ERROR"_cs;

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "U.B."_cs;

			default:
			return "UNKNOWN"_cs;
			}
		} ( );

		con_log( "[OpenGL][% %]: \"%\".", type_str, id, CString{ message, length } );
	}, nullptr );

	// DOESNT FREAKIN WORK!!!
	// 131185 - something about using video card memory for a buffer
	/*
	GLuint const disabled_messages[] ={ 131185 }; // array of ids of messages to disable
	glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 1, disabled_messages, GL_FALSE );
	*/
}

void Window::shutdown()
{
	glfwDestroyWindow( handle );
	glfwTerminate();
}

void Window::close()
{
	glfwWindowShouldClose( handle );
}

void Window::set_title( CString title )
{
	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = ta.get_mark();
	defer{ ta.set_mark( mark ); };

	glfwSetWindowTitle( handle, cstring_to_cstr( title ).data );
}

void Window::set_clear_color( u8 r, u8 g, u8 b )
{
	glClearColor( 1.0f / r, 1.0f / g, 1.0f / b, 1.0f );
}

void Window::clear()
{
	glClear( GL_COLOR_BUFFER_BIT );
}

void Window::display()
{
	glfwSwapBuffers( handle );
}

returning Window::should_close() const -> bool
{
	return glfwWindowShouldClose( handle );
}

returning Window::get_internal_handle() -> GLFWwindow*
{
	return handle;
}
}