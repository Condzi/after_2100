/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/music_source.hpp"
#include "framework/scene/path.hpp"

#include "game/prefabs/enemy_spawner.hpp"
#include "game/prefabs/parallax_background.hpp"
#include "game/prefabs/player.hpp"

using namespace con;

class Level_1 final :
	public Node
{
	CLASS_DEF( Level_1 );

public:
	Level_1();

	void update( r32 dt ) override;

private:
	Player* player;

	bool last_stage{ false };
	Enemy_Spawner* enemy_spawner;
	Path* enemy_path_1;
	Path* enemy_path_2;
	Path* enemy_path_3;

	Path* camera_path;

	Parallax_Background* parallax_background;
	Music_Source* music_source;
};