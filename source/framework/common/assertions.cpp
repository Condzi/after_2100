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
	// We don't want C:/the/whole/path/, just the source/path/
	std::string local_path{ file };
	local_path = local_path.substr( local_path.find( "source" ) );

	if ( condition ) {

		Debug_Log::get_instance().engine_logger->log( log_level, "==============================" );
		Debug_Log::get_instance().engine_logger->log( log_level, "Expression: {}", condition_string );
		Debug_Log::get_instance().engine_logger->log( log_level, "File: {}", local_path );
		Debug_Log::get_instance().engine_logger->log( log_level, "Line: {}", line );
		Debug_Log::get_instance().engine_logger->log( log_level, "==============================" );

		force_break( should_break );
	}

	return condition;
}
}