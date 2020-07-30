#pragma once

#include <chrono>

#include <engine/typedefs.hpp>

namespace con
{
using Clock       = std::chrono::high_resolution_clock;
using Time_Period = std::chrono::microseconds;

namespace Time
{
returning now() -> Time_Period;

returning to_microseconds( Time_Period const& time_period ) -> s64;
returning to_milliseconds( Time_Period const& time_period ) -> s32;
returning to_seconds( Time_Period const& time_period )		-> f32;

returning from_microseconds( s64 time_period ) -> Time_Period;
returning from_milliseconds( s32 time_period ) -> Time_Period;
returning from_seconds( f32 time_period )	   -> Time_Period;

returning difference( Time_Period const& start, Time_Period const& end ) -> Time_Period;
}
}