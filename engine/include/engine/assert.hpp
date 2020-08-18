#pragma once

#include <engine/macro_config.hpp>
#include <engine/typedefs.hpp>
#include <engine/cstring.hpp>

namespace con::priv
{
// The arguments must be null terminated!
void on_assert_fail( CString file, CString line, CString function_name, CString condition );
void on_check_fail( CString file, CString line, CString function_name, CString condition );
void on_glfw_error( s32 error_code, char const* message );
}

// @Important: use on_assert_fail in the con_assert.

#if CON_DEBUG
#define con_assert( x )  if ( !( x ) ) { con::priv::on_assert_fail( __FILE__, CON_STR( __LINE__ ), __func__, #x ); }
#else
#define con_assert( x ) 
#endif

#define release_con_assert( x )  if ( !( x ) ) { con::priv::on_assert_fail( __FILE__, CON_STR( __LINE__ ), __func__, #x ); }