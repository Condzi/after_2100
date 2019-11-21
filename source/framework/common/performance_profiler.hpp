/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

namespace con::priv
{
class Performance_Profiler final
{
public:
	struct Profile final
	{
		std::string_view name;
		u64 start, end;
		u32 thread_id;
	};

	static Performance_Profiler& get_instance();

	void begin_session( std::string_view name, std::string_view file = "performance.json" );
	void end_session();
	void write_profile( Profile profile );

private:
	std::string   session_name;
	std::ofstream output_file;
	u32           profile_count;
};

class Performance_Timer final
{
public:
	Performance_Timer( std::string_view name_ );
	~Performance_Timer();

private:
	std::string name;

	std::chrono::high_resolution_clock::time_point start_timepoint;
};
}

#define G_Performance_Profiler con::priv::Performance_Profiler::get_instance()

#define DO_PROFILING 0

#define COMBINE1(X,Y) X##Y  
#define COMBINE(X,Y) COMBINE1(X,Y)

#if DO_PROFILING

#define G_Profile_Scope( name ) con::priv::Performance_Timer COMBINE( performance_timer_, __LINE__) { name };
#define G_Profile_Function()    G_Profile_Scope( __FUNCTION__ );

#else

#define G_Profile_Scope( name )
#define G_Profile_Function()   

#endif