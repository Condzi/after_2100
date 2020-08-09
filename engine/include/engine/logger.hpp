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

	void push_indent();
	void pop_indent();

	void log( CString message );
	void log_no_indent( CString message );
	void reset_buffer();

	// Returns \0 terminated CString.
	returning get_buffer() const -> CString;

private:
	compile_constant buffer_size = CON_LOGGER_BUFFER_RESERVED_MEMORY;
	char* begin                  = nullptr;
	char const* end              = nullptr;
	char* next_free_slot         = nullptr;
	s32 current_indent           = 0;
};
}

//
// Logging macros. Use con_push/pop_indent to control indentation.
//

#define con_push_indent()             Context.logger->push_indent()
#define con_pop_indent()              Context.logger->pop_indent()
#define con_log( msg, ... )           Context.logger->log( con::sprint( msg, __VA_ARGS__ ) )
#define con_log_no_indent( msg, ... ) Context.logger->log_no_indent( con::sprint( msg, __VA_ARGS__ ) )