/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "framework/common/debug_log.hpp"
#include "framework/common/assertions.hpp"

namespace con::priv
{
Debug_Log& Debug_Log::get_instance()
{
	static Debug_Log* instance = new Debug_Log;

	return *instance;
}

Debug_Log::Debug_Log()
{
	using namespace spdlog;
	using namespace spdlog::sinks;
	using namespace spdlog::level;

	try {
		auto console_sink{ std::make_shared<stdout_color_sink_mt>() };
		auto file_sink{ std::make_shared<basic_file_sink_mt>( LOG_FILE_NAME, true ) };

		console_sink->set_pattern( "[%^%n%$][%T:%e] %v" );
		file_sink->set_pattern( "[%n][%L][%T:%e] %v" );

		engine_logger = std::make_shared<logger>( "ENGINE", sinks_init_list{ console_sink, file_sink } );
		game_logger = std::make_shared<logger>( " GAME ", sinks_init_list{ console_sink, file_sink } );

		engine_logger->flush_on( level_enum::info );
		game_logger->flush_on( level_enum::info );

	} catch ( const std::exception& ex ) {
		std::cout << "\n\n Error initializing spdlog: " << ex.what() << " \n\n";
	}
}
}