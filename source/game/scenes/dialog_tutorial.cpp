/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "dialog_tutorial.hpp"
#include "game_master.hpp"
#include "level_1.hpp"

Dialog_Tutorial::Dialog_Tutorial()
{
	name = "dialog_tutorial";

	actor_image->set_texture_from_name( "portrait_commander" );
	character_name->string.set_locale_name( "#loc_name_commander" );
	character_name->update_vertices();
	dialog_text->string.set_locale_name( "#loc_tutorial_1" );
	current_dialog = 1;
	selected_response = 1;
	show_responses = false;
	arrow_image->set_global_position( arrow_choice_response_position );

	add_responses( { "#loc_tutorial_response_1", "#loc_tutorial_response_2" } );

	needs_update = true;

	update_responses_visibility();
}

void Dialog_Tutorial::input( sf::Event const& event )
{
	log_info( "character_height = {}", character_height );
	if ( sf::Event::KeyPressed is event.type ) {
		if ( sf::Keyboard::E is event.key.code and show_responses ) {
			if ( selected_response is 1 ) {
				dialog_text->string.set_locale_name( "#loc_tutorial_4" );
				current_dialog = 4;
			} else if ( selected_response is 2 ) {
				current_dialog = 3;
				dialog_text->string.set_locale_name( "#loc_tutorial_3" );
			}

			selected_response = 1;
			show_responses = false;
			needs_update = true;
			update_responses_visibility();
		}

		else if ( sf::Keyboard::W is event.key.code and show_responses ) {
			if ( selected_response is 2 ) {
				selected_response = 1;
				arrow_image->set_global_position( arrow_choice_response_position );
				log_info( "arrow.y = {}", arrow_image->get_global_bounds().position.y );
			}
		}

		else if ( sf::Keyboard::S is event.key.code and show_responses ) {
			if ( selected_response is 1 ) {
				selected_response = 2;
				arrow_image->set_global_position( { arrow_choice_response_position.x, arrow_choice_response_position.y + character_height } );
			}
		}

		else if ( sf::Keyboard::Space is event.key.code and not show_responses ) {
			if ( current_dialog is 1 or current_dialog is 3 ) {
				show_responses = true;
				current_dialog = 2;
				dialog_text->string.set_locale_name( "#loc_tutorial_2" );
				selected_response = 1;
				needs_update = true;
				update_responses_visibility();
			} else if ( current_dialog is 4 )
				get_node( "root/game_master" )->cast_to<Game_Master>()->change_level<Level_1>();

		}

		log_info( "Selected response = {}", selected_response );
	}
}

void Dialog_Tutorial::update( r32 dt )
{
	if ( needs_update ) {
		dialog_text->update_vertices( CHARACTER_LIMIT_PER_LINE );
		needs_update = false;
	}
}
