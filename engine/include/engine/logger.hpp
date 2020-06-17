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
	void static set_instance( Logger* instance_ );
	returning static get_instance() -> Logger&;

	void initialize();
	void shutdown();

	void log( CString message, s32 indent = 0 );
	void reset_buffer();

	returning get_buffer() -> CString;

private:
	inline static Logger* instance = nullptr;
	CString buffer;
	char* next_free_slot;
};
}

// @ToDo: Maybe a part of Context? This way we could have different loggers for different parts of the game
// for example. But maybe it's too fancy?
#define G_Logger  con::Logger::get_instance()
#define con_log_indented( indent, msg, ... )  G_Logger.log( con::sprint( msg "\n", __VA_ARGS__ ), indent )
#define con_log( msg, ... ) con_log_indented( 0, msg, __VA_ARGS__ )