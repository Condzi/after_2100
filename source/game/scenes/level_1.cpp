/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "level_1.hpp"
#include "game/prefabs/player.hpp"

Level_1::Level_1()
{
	name = "level_1";
	log_info( "{} instantiated.", name );

	attach( Player::instantiate() );
}
