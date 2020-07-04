#include <windows.h>
#include <intrin.h>
#include <debugapi.h>
#include <WinUser.h>

#include <cstdlib>
#include <cstdio>

#include <engine/assert.hpp>
#include <engine/cstring.hpp>

namespace con::priv
{
file_scope
{
static char assert_message_buffer[CON_ASSERT_MESSAGE_BUFFER_SIZE] = { 0 };
}

bool on_assert_fail( char const* const file, char const* const line, char const* const function_name, char const* const condition )
{
	// Cutting off the whole "D:/dev/after2100/" (or other, that may be different
	// on different enviroments) part by using __FILE__ and subtracting
	// size (length) of the "engine/source/engine/assert.cpp" string, which we know that
	// we don't change.
	compile_constant skip_file_chars = CString{ __FILE__ }.size - CString{ "engine/source/engine/assert.cpp" }.size;

	if ( IsDebuggerPresent() ) {
		constant chars_written = std::snprintf( assert_message_buffer, CON_ASSERT_MESSAGE_BUFFER_SIZE, R"(
=== === === === === === === === === ===
			Assertion failed!

	"%s":%s, %s
	"%s"
=== === === === === === === === === === 
)", file + skip_file_chars, line, function_name, condition );

		release_con_assert( chars_written < CON_ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		OutputDebugStringA( assert_message_buffer );

		__debugbreak();
	} else {
		constant chars_written = std::snprintf( assert_message_buffer, CON_ASSERT_MESSAGE_BUFFER_SIZE, R"(
Please, take a screenshot of this dialog box and send it to the devs. Also, please save the log file which you can find in:
"<game directory>\%s" folder.
Thank you!

===== Debug info =====
"%s":%s, %s
"%s"
)", CON_LOGS_FOLDER, file + skip_file_chars, function_name, line, condition );

		// Bad idea?
		release_con_assert( chars_written < CON_ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		MessageBoxA(
			NULL,
			assert_message_buffer,
			"Serious Game Error!",
			MB_TOPMOST | MB_ICONERROR | MB_OK
		);

		std::exit( EXIT_FAILURE );
	}

	return true;
}

void on_glfw_error( s32 error_code, char const* message )
{
	if ( IsDebuggerPresent() ) {
		constant chars_written = std::snprintf( assert_message_buffer, CON_ASSERT_MESSAGE_BUFFER_SIZE, R"(
=== === === === === === === === === ===
			GLFW Error!
	
	Error code: %i
	"%s"
=== === === === === === === === === === 
)", error_code, message );

		release_con_assert( chars_written < CON_ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		OutputDebugStringA( assert_message_buffer );

		__debugbreak();
	} else {
		constant chars_written = std::snprintf( assert_message_buffer, CON_ASSERT_MESSAGE_BUFFER_SIZE, R"(
Please, take a screenshot of this dialog box and send it to the devs. Also, please save the log file which you can find in: 
"<game directory>\%s" folder.
Thank you!

===== Debug info =====
GLFW Error!
Error code: %i
"%s"
)", CON_LOGS_FOLDER, error_code, message );

		// Bad idea?
		release_con_assert( chars_written < CON_ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		MessageBoxA(
			NULL,
			assert_message_buffer,
			"Serious Game Error!",
			MB_TOPMOST | MB_ICONERROR | MB_OK
		);

		std::exit( EXIT_FAILURE );
	}
}
}