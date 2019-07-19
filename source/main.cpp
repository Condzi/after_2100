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
	// Have to split it to G_Window.initialize and G_App.initialize() because
	// root has fps_counter label that is positioned relative to window that doesn't
	// exist yet.
	G_Window.initialize( 1280, 720, 64, "after_2100" );

	G_App.initialize();
	G_Root.change_scene( Level_1::instantiate() );
	G_App.run();
}