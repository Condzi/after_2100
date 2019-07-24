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
#include "framework/common/typedefs.hpp"

#include "framework/common/vec2.hpp"
#include "framework/common/percent_position.hpp"

// Probably from spdlog.
#undef TRANSPARENT

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
	if ( val > 360 ) {
		auto const as_integer{ static_cast<con::s32>( val ) };
		return static_cast<con::r32>( val ) - 360 * ( as_integer % 360 );
	}

	// add support for it if needed
	if( val < 0 ) assert( "Negative degrees are not supported." );

	return val;
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
template<class... Ts> visitor_overload( Ts... ) -> visitor_overload<Ts...>;
}