/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/common/debug_log.hpp"

int main()
{
	log_info( "Info! 123 = {}.", 123 );
	log_warning( "Warning! 456 = {}.", 456 );
	log_error( "Error! 789 = {}.", 789 );

	engine_log_info( "Info! 321 = {}.", 321 );
	engine_log_warning( "Warning! 654 = {}.", 654 );
	engine_log_error( "Error! 987 = {}.", 987 );
}