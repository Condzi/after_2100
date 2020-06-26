#pragma once

#include <engine/context.hpp>
#include <engine/typedefs.hpp>
#include <engine/cstring.hpp>
#include <engine/formatting.hpp>

namespace con
{
class Logger final
{
public:
	void initialize();
	void shutdown();

	void log( CString message, s32 indent = 0 );
	void reset_buffer();

	// Returns \0 terminated CString.
	returning get_buffer() -> CString;

private:
	inline static Logger* instance = nullptr;
	CString buffer;
	char* next_free_slot;
};
}

// Logs 'msg' with '...' args to Context.logger with 'indent' indentation
#define con_log_indented( indent, msg, ... )  Context.logger->log( con::sprint( msg "\n", __VA_ARGS__ ), indent )
// Logs 'msg' with '...' args to Context.logger with no indentation
#define con_log( msg, ... ) con_log_indented( 0, msg, __VA_ARGS__ )