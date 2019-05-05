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
#include <vector>

#include <nlohmann/json.hpp>

#include "framework/common/debug_log.hpp"

// Probably from spdlog.
#undef TRANSPARENT

namespace con
{
using s8 =  int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8 =  uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using r32 = float;
using r64 = double;
}

#define CLASS_DEF( class_ )                                        \
class_() = default;                                                \
public:                                                            \
                                                                   \
static constexpr const char* get_class_name_static()               \
{                                                                  \
	return #class_;                                                \
}                                                                  \
                                                                   \
virtual std::string get_class_name() const                         \
{                                                                  \
	return get_class_name_static();                                \
}                                                                  \
                                                                   \
inline static std::unique_ptr<class_> instantiate()                \
{                                                                  \
	return std::make_unique<class_>();                             \
}                                                                  \
private:
                                                                   
