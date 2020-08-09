#include <windows.h>
#include <intrin.h>
#include <debugapi.h>
#include <WinUser.h>

#include <cstdlib>
#include <cstdio>

#include <engine/assert.hpp>
#include <engine/cstring.hpp>

// We close window on failed assertion.
#include <engine/window.hpp>
#include <engine/context.hpp>

#include <engine/logger.hpp>

namespace con::priv
{
bool on_assert_fail( char const* const file, char const* const line, char const* const function_name, char const* const condition )
{
	// Cutting off the whole "D:/dev/after2100/" (or other, that may be different
	// on different enviroments) part by using __FILE__ and subtracting
	// size (length) of the "engine/source/engine/assert.cpp" string, which we know that
	// we don't change.
	compile_constant skip_file_chars = CString{ __FILE__ }.size - CString{ "engine/source/engine/assert.cpp" }.size;

	Context.window->close();
	Context.exit_flags.requested_by_app = true;

	compile_constant message_format = CString{ R"(
=== === === === === === === === === ===
			Assertion failed!

	Location:  "%", line %
	Function:  %
	Condition: "%"
=== === === === === === === === === ===
)"};

	constant message_to_print = sprint( message_format, cstring_from_cstr( file + skip_file_chars ), cstring_from_cstr( line ), cstring_from_cstr( function_name ), cstring_from_cstr( condition ) );

	con_log_no_indent( "\n%\n", message_to_print );

	constant message_to_print_null_terminated = sprint( "%\0", message_to_print );

	if ( IsDebuggerPresent() ) {
		OutputDebugStringA( message_to_print_null_terminated.data );

		__debugbreak();
	} else {
		compile_constant non_debugger_message_format = CString{ R"(
Please, take a screenshot of this dialog box and send it to the devs. Also, please save the log file which you can find in:
"<game directory>\%" folder.
Thank you!

===== Debug info =====
Location:  "%", line %
Function:  %
Condition: "%"
)" "\0" };
		constant non_debugger_message_to_print = sprint( non_debugger_message_format, CString{ CON_LOGS_FOLDER },  cstring_from_cstr( file + skip_file_chars ), cstring_from_cstr( line ), cstring_from_cstr( function_name ), cstring_from_cstr( condition ) );

		MessageBoxA(
			NULL,
			non_debugger_message_to_print.data,
			"Serious Game Error!",
			MB_TOPMOST | MB_ICONERROR | MB_OK
		);
	}

	return true;
}

void on_glfw_error( s32 error_code, char const* message )
{
	Context.window->close();
	Context.exit_flags.requested_by_app = true;

	compile_constant message_format = CString{ R"(
=== === === === === === === === === ===
			GLFW Error!
	
	Error code: %
	"%"
=== === === === === === === === === === 
)" };

	constant message_to_print = sprint( message_format, error_code, cstring_from_cstr( message ) );

	con_log_no_indent( "\n%\n", message_to_print );
	
	if ( IsDebuggerPresent() ) {
		constant message_to_print_null_terminated = sprint( "%\0", message_to_print );
		OutputDebugStringA( message_to_print_null_terminated.data );

		__debugbreak();
	} else {
		compile_constant non_debbuger_message_format = CString{ R"(
Please, take a screenshot of this dialog box and send it to the devs. Also, please save the log file which you can find in: 
"<game directory>\%s" folder.
Thank you!

===== Debug info =====
GLFW Error!
Error code: %
"%"
)" "\0" };
		constant non_debbuger_message_to_print = sprint( non_debbuger_message_format, CString{ CON_LOGS_FOLDER }, error_code, cstring_from_cstr( message ) );

		MessageBoxA(
			NULL,
			non_debbuger_message_to_print.data,
			"Serious Game Error!",
			MB_TOPMOST | MB_ICONERROR | MB_OK
		);
	}
}
}