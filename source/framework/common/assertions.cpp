/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <intrin.h>

#include "framework/common/assertions.hpp"
#include "framework/common/debug_log.hpp"

namespace con::priv
{
bool force_break( bool condition )
{
	if ( condition )
		__debugbreak();

	return condition;
}

bool report_and_break( spdlog::level::level_enum log_level, bool should_break, bool condition, const char * condition_string, const char * file, s64 line )
{
	constexpr const char* MESSAGE{ "\"{}\" failed in file \"{}\" in line {}." };
	if ( condition ) {
		Debug_Log::get_instance().engine_logger->log( log_level, MESSAGE, condition_string, file, line );

		if ( should_break )
			force_break( true );
	}

	return condition;
}
}