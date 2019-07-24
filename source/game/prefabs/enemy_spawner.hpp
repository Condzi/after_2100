/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "enemy_base.hpp"

class Enemy_Spawner :
	public Node
{
	CLASS_DEF( Enemy_Spawner );

public:
	r32 spawn_interval{ 2.0sec };

	Enemy_Spawner();

	template <typename TEnemy>
	void set_enemy_type()
	{
		static_assert( std::is_base_of_v<Enemy_Base, TEnemy> );
		spawning_function = [this]() -> Enemy_Base& {
			return *attach( TEnemy::instantiate() )->cast_to<Enemy_Base>();
		};
	}

	void set_spawn_limit( u32 limit );
	void set_path( Path const& path );

	[[nodiscard]] auto get_spawn_limit() const           -> u32;
	[[nodiscard]] auto get_already_spawned_count() const -> u32;
	[[nodiscard]] auto is_spawning() const               -> bool;
	[[nodiscard]] auto is_finished() const               -> bool;

	void start();
	void stop();
	void reset();

	void update( r32 delta ) override;

private:
	using Spawning_Function = std::function<Enemy_Base&()>;

	Spawning_Function spawning_function;
	Path const*       path_to_follow{ nullptr };
	r32               time_since_last_spawn{ 0 };
	u32               spawn_limit{ 0 };
	u32               already_spawned_count{ 0 };
	bool              spawning{ false };
};