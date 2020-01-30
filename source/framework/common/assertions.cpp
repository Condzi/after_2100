/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "framework/common/debug_log.hpp"

#include <intrin.h>

namespace con::priv
{
bool force_break( bool condition )
{
	if ( condition )
		__debugbreak();

	return condition;
}

bool report_and_break( spdlog::level::level_enum log_level, bool should_break, bool condition, std::string_view condition_string, std::string_view file, s64 line )
{
	if ( condition is false )
		return false;

	static constant local_path_begin = [&]() {
		return file.find( "source" );
	}( );

	constant file_path_length = file.size();
	std::string_view const local_path{ file.data() + local_path_begin, file_path_length - local_path_begin };

	auto& logger = Debug_Log::get_instance().engine_logger;

	logger->log( log_level, "==============================" );
	logger->log( log_level, "Expression: {}", condition_string );
	logger->log( log_level, "File: {}", local_path );
	logger->log( log_level, "Line: {}", line );
	logger->log( log_level, "==============================" );

	force_break( should_break );

	return true;
}
}