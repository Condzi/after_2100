#include <windows.h>
#include <intrin.h>
#include <debugapi.h>
#include <WinUser.h>

#include <cstdlib>
#include <cstdio>

#include <engine/assert.hpp>

namespace con::priv
{
file_scope 
{
compile_constant ASSERT_MESSAGE_BUFFER_SIZE = 512;
static char assert_message_buffer[ASSERT_MESSAGE_BUFFER_SIZE] = { 0 };
}

bool on_assert_fail( char const* const file, char const* const line, char const* const condition )
{
	// So because I'm a solo dev I can safely assume that __FILE__ always refers to the same directory:
	// D:/programming/games/after_2100/...
	compile_constant skip_file_chars = 32;

	if ( IsDebuggerPresent() ) {
		constant chars_written = std::snprintf( assert_message_buffer, ASSERT_MESSAGE_BUFFER_SIZE, R"(
=== === === === === === === === === ===
			Assertion failed!

	"%s":%s
	"%s"
=== === === === === === === === === === 
)", file + skip_file_chars, line, condition );

		release_con_assert( chars_written < ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		OutputDebugStringA( assert_message_buffer );

		__debugbreak();
	} else {
		constant chars_written = std::snprintf( assert_message_buffer, ASSERT_MESSAGE_BUFFER_SIZE, R"(
Please, take a screenshot of this dialog box and send it to the devs. Also, please save the log file which you can find in:
"<game directory>\%s" folder.
Thank you!

===== Debug info =====
"%s":%s
"%s"
)", CON_LOGS_FOLDER, file + skip_file_chars, line, condition );

		// Bad idea?
		release_con_assert( chars_written < ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		MessageBoxA(
			NULL,
			assert_message_buffer,
			"Serious Game Error!",
			MB_TOPMOST | MB_ICONERROR | MB_OK
		);


		// @Robustness: Call shutdown of Window, Logger and other stuff
		std::exit( EXIT_FAILURE );
	}

	return true;
}

void on_glfw_error( s32 error_code, char const* message )
{
	if ( IsDebuggerPresent() ) {
		constant chars_written = std::snprintf( assert_message_buffer, ASSERT_MESSAGE_BUFFER_SIZE, R"(
=== === === === === === === === === ===
			GLFW Error!
	
	Error code: %i
	"%s"
=== === === === === === === === === === 
)", error_code, message );

		release_con_assert( chars_written < ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		OutputDebugStringA( assert_message_buffer );

		__debugbreak();
	} else {
		constant chars_written = std::snprintf( assert_message_buffer, ASSERT_MESSAGE_BUFFER_SIZE, R"(
Please, take a screenshot of this dialog box and send it to the devs. Also, please save the log file which you can find in: 
"<game directory>\%s" folder.
Thank you!

===== Debug info =====
GLFW Error!
Error code: %i
"%s"
)", CON_LOGS_FOLDER, error_code, message );

		// Bad idea?
		release_con_assert( chars_written < ASSERT_MESSAGE_BUFFER_SIZE );
		release_con_assert( chars_written > 0 );

		MessageBoxA(
			NULL,
			assert_message_buffer,
			"Serious Game Error!",
			MB_TOPMOST | MB_ICONERROR | MB_OK
		);


		// @Robustness: Call shutdown of Window, Logger and other stuff
		std::exit( EXIT_FAILURE );
	}
}
}