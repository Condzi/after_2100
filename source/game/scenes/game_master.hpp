/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/node.hpp"

using namespace con;

class Game_Master :
	public Node
{
	CLASS_DEF( Game_Master );

public:
	Game_Master();

	void input( sf::Event const& ev ) override;

	template <typename TNode>
	void change_level()
	{
		level = attach<TNode>();
		log_info( "Switching levels: {}.", level->name );
	}

	Node* get_level();

	void update_pause_mode();

	void update( r32 dt ) override;

private:
	Node* pause_screen;
	Node* level{ nullptr };
};