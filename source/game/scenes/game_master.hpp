/*
	Conrad 'Condzi' Kubacki 2020
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
		level_instantiating_function = [this] {
			if ( level )
				level->queue_for_delete();

			level = attach<TNode>();
		};

		level_instantiating_function();
		log_info( "Switching levels: {}.", level->name );
	}

	Node* get_level();
	void reset_level();

	void update_pause_mode();

	void update( r32 dt ) override;

private:
	Node* pause_screen;
	Node* failure_screen;
	Node* success_screen;
	Node* level{ nullptr };

	// We have to somethow re-instatntiate the level after player's failure.
	std::function<void()> level_instantiating_function;
};