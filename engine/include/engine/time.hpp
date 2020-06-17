#pragma once

#include <chrono>

#include <engine/typedefs.hpp>

namespace con
{
using Clock       = std::chrono::high_resolution_clock;
using Time_Period = std::chrono::microseconds;

namespace Time
{
[[nodiscard]] auto now() -> Time_Period;

[[nodiscard]] auto to_microseconds( Time_Period const& time_period ) -> s64;
[[nodiscard]] auto to_milliseconds( Time_Period const& time_period ) -> s32;
[[nodiscard]] auto to_seconds( Time_Period const& time_period )		 -> f32;

[[nodiscard]] auto from_microseconds( s64 time_period ) -> Time_Period;
[[nodiscard]] auto from_milliseconds( s32 time_period ) -> Time_Period;
[[nodiscard]] auto from_seconds( f32 time_period )		-> Time_Period;

[[nodiscard]] auto difference( Time_Period const& start, Time_Period const& end ) -> Time_Period;
}
}