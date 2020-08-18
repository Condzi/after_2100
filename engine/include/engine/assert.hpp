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

struct Check_Helper final
{
	bool const failed     : 1 = false;
	bool const not_failed : 1 = true;

	Check_Helper( bool condition, CString file, CString line, CString function_name, CString condition_str ) :
		failed(     condition == false ),
		not_failed( condition == true )
	{
		if ( failed ){
			on_check_fail( file, line, function_name, condition_str );
		}
	}
};
}

#if CON_DEBUG
#define con_assert( x )  if ( !( x ) ) { con::priv::on_assert_fail( __FILE__, CON_STR( __LINE__ ), __func__, #x ); }
#else
#define con_assert( x ) 
#endif

#define release_con_assert( x )  if ( !( x ) ) { con::priv::on_assert_fail( __FILE__, CON_STR( __LINE__ ), __func__, #x ); }

#if CON_DO_CHECKS == true
//
// It's your decision to handle the failure.
//
//		if ( con_check( false ).failed ) { exit or something...}
//
// or 
//
//		if ( con_check( buffer != nullptr ).not_failed ) { do stuff with valid buffer }
//
#define con_check( x )  con::priv::Check_Helper{ x, __FILE__, CON_STR( __LINE__ ), __func__, #x }
#else
#define con_check( x )
#endif