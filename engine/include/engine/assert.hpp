#pragma once

#include <engine/macro_config.hpp>
#include <engine/typedefs.hpp>

namespace con::priv
{
[[noreturn]] bool on_assert_fail( char const* const file, char const* const line, char const* const condition );
[[noreturn]] void on_glfw_error( s32 error_code, char const* message );
}

#if CON_DEBUG
// @Robustess: Add math_conassert
#define con_assert( x )  unused( ( !!( x ) ) || ( con::priv::on_assert_fail( __FILE__, CON_STR( __LINE__ ), #x ) ) )
#else
#define con_assert( x ) 
#endif

#define release_con_assert( x )  unused( ( !!( x ) ) || ( con::priv::on_assert_fail( __FILE__, CON_STR( __LINE__ ), #x ) ) )