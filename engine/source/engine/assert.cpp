#include <windows.h>
#include <intrin.h>
#include <debugapi.h>
#include <WinUser.h>

#include <cstdlib>
#include <cstdio>

#include <engine/assert.hpp>
#include <engine/cstring.hpp>

// We shutdown the window on failure.
#include <engine/window.hpp>
#include <engine/context.hpp>

#include <engine/logger.hpp>

namespace con::priv
{
//
// When assertion fails and we are in debug mode, we want to open the crash file.
// We do that using Windows commands.
//
void open_crash_log_in_text_editor()
{
	// This way the file opens in the default editor.
	std::system( CON_LOGS_FOLDER "\\crash.txt" );
}

// Cutting off the whole "D:/dev/after2100/" part (or other, that may be different
// in different enviroments) by using __FILE__ and subtracting
// length of the "engine/source/engine/assert.cpp" string, which we know that
// we won't change.
compile_constant skip_file_chars = CString{ __FILE__ }.size - CString{ "engine/source/engine/assert.cpp" }.size;

void on_assert_fail( CString file, CString line, CString function_name, CString condition )
{
	file = CString{ file.data + skip_file_chars, file.size - skip_file_chars };

	// If we wouldn't close the window we would stuck in the fullscreen mode.
	Context.window->shutdown();

	//
	// Save the info to the crash file.
	//
	compile_constant message_format = CString{ R"(
!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!
			Assertion failed!

	Location:  "%s", line %s
	Function:  %s
	Condition: "%s"
!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!
)" "\0" };

	compile_constant message_buffer_size = CON_ASSERT_MESSAGE_BUFFER_SIZE;
	char message_buffer[message_buffer_size] = { 0 };


	snprintf( message_buffer, message_buffer_size, message_format.data, file.data, line.data, function_name.data, condition.data );

	//
	// Save the log to CON_LOGS_FOLDER "crash.txt". 
	//

	auto crash_file      = fopen( CON_LOGS_FOLDER "crash.txt", "wb" );
	constant data_to_log = Context.logger->get_buffer();

	fputs( "\tCrash file start.\n\n", crash_file );
	fputs( data_to_log.data,          crash_file );
	fputs( message_buffer,            crash_file );
	fputs( "\n\tCrash file end.",     crash_file );

	fclose( crash_file );

	//
	// In release mode we also do an error box.
	//
#if CON_DEBUG == false
	memset( message_buffer, 0, message_buffer_size * sizeof( char ) );

	compile_constant non_debugger_message_format = CString{ R"(
It's not good... Sorry for that. 
Could you send a screenshot of this error box to the developer, please? 
If you could send us the "crash.txt" file too, it would be awesome! 
It's in the "<game directory>\%s" folder.
Thank you!

===== Debug info =====
Location:  "%s", line %s
Function:  %s
Condition: "%s"
======================
)" "\0" };

	snprintf( message_buffer, message_buffer_size, non_debugger_message_format.data, CON_LOGS_FOLDER, file.data, line.data, function_name.data, condition.data );

	MessageBoxA(
		NULL,
		message_buffer,
		"Yup, we crashed.",
		MB_TOPMOST | MB_ICONERROR | MB_OK
	);
#else
	open_crash_log_in_text_editor();

	if ( IsDebuggerPresent() ){
		OutputDebugStringA( message_buffer );

		__debugbreak();
	}
#endif

	std::exit( EXIT_FAILURE );
}

void on_check_fail( CString file, CString line, CString function_name, CString condition )
{
	file = CString{ file.data + skip_file_chars, file.size - skip_file_chars };

	compile_constant message_format = CString{ R"(
=== === === === === === === === === ===
			Check failed!

	Location:  "%", line %
	Function:  %
	Condition: "%"
=== === === === === === === === === ===
)" };

	constant message_to_print = sprint( message_format, file, line, function_name, condition );

	con_log_no_indent( "\n%\n", message_to_print );

	if ( IsDebuggerPresent() ){
		constant message_to_print_null_terminated = sprint( "%\0", message_to_print );
		OutputDebugStringA( message_to_print_null_terminated.data );

		__debugbreak();
	}
}

void on_glfw_error( s32 error_code, char const* message )
{
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

	if ( IsDebuggerPresent() ){
		constant message_to_print_null_terminated = sprint( "%\0", message_to_print );
		OutputDebugStringA( message_to_print_null_terminated.data );

		__debugbreak();
	} else{
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