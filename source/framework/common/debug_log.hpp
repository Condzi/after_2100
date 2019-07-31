/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include <spdlog/logger.h>

namespace con::priv
{
class Debug_Log final
{
public:
	static constexpr const char* LOG_FILE_NAME{ "log.txt" };

	enum Level
	{
		Info     = spdlog::level::level_enum::info,
		Warning  = spdlog::level::level_enum::warn,
		Error    = spdlog::level::level_enum::err,
		Critical = spdlog::level::level_enum::critical,
	};

	std::unique_ptr<spdlog::logger> engine_logger;
	std::unique_ptr<spdlog::logger> game_logger;

	//
	//	Logger probably should't've an intialization method like .init():bool because it may be called before (maybe by some global variable?)
	//
	static Debug_Log& get_instance();

private:
	Debug_Log();
	Debug_Log( Debug_Log const& ) = delete;
	Debug_Log( Debug_Log const&& ) = delete;
	Debug_Log& operator=( Debug_Log const& ) = delete;
	Debug_Log& operator=( Debug_Log const&& ) = delete;
};

//
//	Engine-specific logging macros.
//
#define engine_log_info( ... )     con::priv::Debug_Log::get_instance().engine_logger->info( __VA_ARGS__ )
#define engine_log_warning( ... )  con::priv::Debug_Log::get_instance().engine_logger->warn( __VA_ARGS__ )
#define engine_log_error( ... )    con::priv::Debug_Log::get_instance().engine_logger->error( __VA_ARGS__ )
#define engine_log_critical( ... ) con::priv::Debug_Log::get_instance().engine_logger->critical( __VA_ARGS__ )

//
//	Use this macros to log info about game.
//
#define log_info( ... )     con::priv::Debug_Log::get_instance().game_logger->info( __VA_ARGS__ )
#define log_warning( ... )  con::priv::Debug_Log::get_instance().game_logger->warn( __VA_ARGS__ )
#define log_error( ... )    con::priv::Debug_Log::get_instance().game_logger->error( __VA_ARGS__ )
#define log_critical( ... ) con::priv::Debug_Log::get_instance().game_logger->critical( __VA_ARGS__ )
}