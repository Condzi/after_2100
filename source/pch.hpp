/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#define DO_PROFILING 0

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <unordered_map>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

#include "framework/common/debug_log.hpp"
#include "framework/common/global_flags.hpp"
#include "framework/common/percent_position.hpp"
#include "framework/common/performance_profiler.hpp"
#include "framework/common/random.hpp"
#include "framework/common/typedefs.hpp"
#include "framework/common/vec2.hpp"

// Probably from spdlog.
#undef TRANSPARENT
