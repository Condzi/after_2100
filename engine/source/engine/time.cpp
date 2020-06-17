#include <engine/time.hpp>

using namespace std::chrono;

namespace con
{

using my_mc = duration<s64, std::micro>;
using my_ms = duration<s32, std::milli>;
using my_s  = duration<f32>;

auto Time::difference( Time_Period const& start, Time_Period const& end ) -> Time_Period
{
	return { end-start };
}

auto Time::now() -> Time_Period
{
	return Time_Period{ duration_cast<microseconds>( Clock::now().time_since_epoch() ).count() };
}

auto Time::to_microseconds( Time_Period const& time_period ) -> s64
{
	return duration_cast<my_mc>( time_period ).count();
}

auto Time::to_milliseconds( Time_Period const& time_period ) -> s32
{
	return duration_cast<my_ms>( time_period ).count();
}

auto Time::to_seconds( Time_Period const& time_period ) -> f32
{
	return duration_cast<my_s>( time_period ).count();
}

auto Time::from_microseconds( s64 time_period ) -> Time_Period
{
	return Time_Period{ duration_cast<microseconds>( my_mc{ time_period } ) };
}

auto Time::from_milliseconds( s32 time_period )  -> Time_Period
{
	return Time_Period{ duration_cast<microseconds>( my_ms{ time_period } ) };
}

auto Time::from_seconds( f32 time_period ) -> Time_Period
{
	return Time_Period{ duration_cast<microseconds>( my_s{ time_period } ) };
}
}