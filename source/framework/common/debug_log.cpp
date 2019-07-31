/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "framework/common/debug_log.hpp"
#include "framework/common/assertions.hpp"
#include "framework/scene/debug_console.hpp"
#include "framework/scene/application.hpp"

namespace con::priv
{
template <typename TMutex>
class Debug_Log_Sink final :
	public spdlog::sinks::base_sink<TMutex>
{
public:
	void sink_it_( const spdlog::details::log_msg& msg ) override
	{
		fmt::memory_buffer formatted;
		spdlog::sinks::sink::formatter_->format( msg, formatted );
		
		G_Debug_Console.print( fmt::to_string( formatted ) );
	}
	void flush_() override {}
};


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
		auto console_sink{ std::make_shared<Debug_Log_Sink<spdlog::details::null_mutex>>() };
		auto file_sink{ std::make_shared<basic_file_sink_st>( LOG_FILE_NAME, true ) };

		console_sink->set_pattern( "[%^%n%$][%T:%e] %v" );
		file_sink->set_pattern( "[%n][%L][%T:%e] %v" );

		engine_logger = std::make_unique<logger>( "ENGINE", sinks_init_list{ console_sink, file_sink } );
		game_logger = std::make_unique<logger>( " GAME ", sinks_init_list{ console_sink, file_sink } );

		engine_logger->flush_on( level_enum::info );
		game_logger->flush_on( level_enum::info );

	}
	catch ( const std::exception& ex ) {
		std::cout << "\n\n Error initializing spdlog: " << ex.what() << " \n\n";
	}
}
}