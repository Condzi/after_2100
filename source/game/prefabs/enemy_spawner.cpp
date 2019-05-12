/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "enemy_spawner.hpp"

void Enemy_Spawner::set_spawn_limit( u32 limit )
{
	spawn_limit = limit;
}

void Enemy_Spawner::set_path( Path const& path )
{
	path_to_follow = &path;
}

auto Enemy_Spawner::get_spawn_limit() const -> u32
{
	return spawn_limit;
}

auto Enemy_Spawner::get_already_spawned_count() const -> u32
{
	return already_spawned_count;
}

auto Enemy_Spawner::get_is_spawning() const -> bool
{
	return is_spawning;
}

auto Enemy_Spawner::get_is_finished() const -> bool
{
	return spawn_limit > 0 and already_spawned_count == spawn_limit;
}

void Enemy_Spawner::start()
{
	report_warning_if( is_spawning )
	{
		log_warning( "Spawner is already started." );
		return;
	}

	report_error_if( path_to_follow is nullptr )
	{
		return;
	}

	report_error_if( spawn_interval <= 0.0sec )
	{
		return;
	}

	report_error_if( spawning_function is false )
	{
		log_error( "No spawning function set for Enemy_Spawner. (no type was assigned)" );
		return;
	}

	is_spawning = true;
}

void Enemy_Spawner::stop()
{
	report_warning_if( not is_spawning )
	{
		log_warning( "Spawner is already not spawning." );
		return;
	}

	is_spawning = false;
}

void Enemy_Spawner::reset()
{
	time_since_last_spawn = 0;
	already_spawned_count = 0;
}

void Enemy_Spawner::update( r32 delta )
{
	if ( is_spawning is false or get_is_finished() returned true )
		return;

	time_since_last_spawn += delta;

	if ( time_since_last_spawn >= spawn_interval ) {
		time_since_last_spawn -= spawn_interval;
		already_spawned_count++;

		Enemy_Base& enemy = spawning_function();
		enemy.set_path( *path_to_follow );
		enemy.start_following();
	}

	if ( get_is_finished() )
		stop();
}
