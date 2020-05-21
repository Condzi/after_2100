#pragma once

#include <cstdint>

namespace con
{
using u8  = std::uint8_t;
using s8  = std::int8_t;
using u16 = std::uint16_t;
using s16 = std::int16_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using u64 = std::uint64_t;
using s64 = std::int64_t;

using f32 = float;
using f64 = double;

using byte = u8;

#define unused( x )			( ( void )( x ) )
#define constant			auto const
#define compile_constant	static constexpr auto
#define file_scope			namespace

// 
//	Defer's macro stuff here
//
#define CON_CONCAT_INTERNAL( x, y ) x##y
#define CON_CONCAT( x, y ) CON_CONCAT_INTERNAL( x, y )

namespace priv
{
template <typename T>
struct Exit_Scope final
{
	T lambda;
	Exit_Scope( T l ) : lambda( l ) {}
	~Exit_Scope() { lambda(); }
	Exit_Scope( const Exit_Scope& ) = delete;
	Exit_Scope& operator=( const Exit_Scope& ) = delete;
};

struct Exit_Scope_Help final
{
	template <typename T>
	Exit_Scope<T> operator+( T t ) { return Exit_Scope{ t }; }
};

}
#define defer	 constant CON_CONCAT( defer__, __LINE__ ) = con::priv::Exit_Scope_Help{} + [&]
//
// End of defer's macro stuff.
//
}