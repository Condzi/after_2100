/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <memory>
#include <iterator>
#include <set>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

#include "framework/common/debug_log.hpp"
#include "framework/common/typedefs.hpp"

#include "framework/common/vec2.hpp"
#include "framework/common/percent_position.hpp"
#include "framework/common/random.hpp"

// Probably from spdlog.
#undef TRANSPARENT