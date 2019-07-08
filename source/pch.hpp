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
#include "framework/common/debug_flags.hpp"

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
inline static auto instantiate()                                   \
{                                                                  \
	return std::make_unique<class_>();                             \
}                                                                  \
private:


inline constexpr con::r32 operator"" px( long double val )
{
	return static_cast<con::r32>( val );
}

inline constexpr con::r32 operator"" sec( long double val )
{
	return static_cast<con::r32>( val );
}
// Automaticly sets correct degrees value (400 -> 400 - 360 = 40)
inline constexpr con::r32 operator"" deg( long double val )
{
	auto const as_integer{ static_cast<con::s32>( val ) };

	return static_cast<con::r32>( val ) - 360 * ( as_integer % 360 );
}

#define is ==
#define is_not !=
#define returned is
#define constant auto const
#define change_owner( uptr ) std::move( uptr )
#define unused( x ) ( ( void )( x ) )

namespace con
{
// Helps with std::variant visiting. provides possibility to pass multiple lambdas. see: https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts> struct visitor_overload : Ts... { using Ts::operator()...; };
template<class... Ts> visitor_overload(Ts...) -> visitor_overload<Ts...>;
}