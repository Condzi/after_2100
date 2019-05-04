/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con::priv
{
bool force_break( bool condition );
bool report_and_break( spdlog::level::level_enum log_level, bool should_break, bool condition, const char* condition_string, const char* file, s64 line );
}

//
//	The idea is that if an error occurs you can either throw or something. Usage:
//  report_error_if( condition_that_fail )
//  {
//      don't switch scene or something
//  }
//

#define report_info_if( condition )     if( con::priv::report_and_break( spdlog::level::level_enum::info,     false, condition, #condition, __FILE__, __LINE__ ) )
#define report_warning_if( condition )  if( con::priv::report_and_break( spdlog::level::level_enum::warn,     false, condition, #condition, __FILE__, __LINE__ ) )
#define report_error_if( condition )    if( con::priv::report_and_break( spdlog::level::level_enum::err,      true,  condition, #condition, __FILE__, __LINE__ ) )
#define report_critical_if( condition ) if( con::priv::report_and_break( spdlog::level::level_enum::critical, true,  condition, #condition, __FILE__, __LINE__ ) )
#define break_if( condition )           if( con::priv::force_break( condition ) ) 