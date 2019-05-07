/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/scene/application.hpp"
#include "framework/scene/sprite.hpp"
#include "framework/common/resources_storage.hpp"

using namespace con;

int main()
{
	G_App.initialize( 1280, 720, 60, "after_2100" );

	{
		Sprite* sprite_node{ nullptr };
		{
			auto spr = Sprite::instantiate();
			sprite_node = dynamic_cast<Sprite*>( G_Root.attach( change_owner( spr ) ) );
		}
		sprite_node->set_texture( G_Resources_Storage.get_texture( "player" ) );
	}


	G_App.run();
}