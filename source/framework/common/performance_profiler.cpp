/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "performance_profiler.hpp"

namespace con::priv
{
Performance_Timer::Performance_Timer( std::string_view name_ ) :
	name( name_ )
{
	start_timepoint = std::chrono::high_resolution_clock::now();
}

Performance_Timer::~Performance_Timer()
{
	constant end_timepoint = std::chrono::high_resolution_clock::now();

	u64 const start_us = std::chrono::time_point_cast<std::chrono::microseconds>( start_timepoint ).time_since_epoch().count();
	u64 const end_us   = std::chrono::time_point_cast<std::chrono::microseconds>( end_timepoint ).time_since_epoch().count();

	u32 const thread_id = std::hash<std::thread::id>{}( std::this_thread::get_id() );

	G_Performance_Profiler.write_profile( { name, start_us, end_us, thread_id } );

}

Performance_Profiler& Performance_Profiler::get_instance()
{
	static Performance_Profiler* instance = new Performance_Profiler;

	return *instance;
}

void Performance_Profiler::begin_session( std::string_view name, std::string_view file )
{
	output_file.open( file );

	output_file << R"({"otherData": {},"traceEvents":[)";
	output_file.flush();
}

void Performance_Profiler::end_session()
{
	output_file << "]}";
	output_file.flush();
	output_file.close();
	session_name = "<no_session>";
	profile_count = 0;
}

void Performance_Profiler::write_profile( Profile profile )
{
	if ( profile_count++ > 0 )
		output_file << ',';

	std::string name{ profile.name };
	std::replace( name.begin(), name.end(), '"', '\'' );

	output_file << "{";
	output_file << R"("cat":"function",)";
	output_file << R"("dur":)" << ( profile.end - profile.start ) << ',';
	output_file << R"("name":")" << name << R"(",)";
	output_file << R"("ph":"X",)";
	output_file << R"("pid":0,)";
	output_file << R"("tid":)" << profile.thread_id << ",";
	output_file << R"("ts":)" << profile.start;
	output_file << "}";

	output_file.flush();
}
}