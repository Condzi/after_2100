/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/scene/application.hpp"

#include "game/scenes/level_1.hpp"

using namespace con;

int main()
{
	G_App.initialize( 1280, 720, 60, "after_2100" );
	G_Root.change_scene( Level_1::instantiate() );
	G_App.run();
}