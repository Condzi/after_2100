/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "dialog_template.hpp"
#include "framework/common/resources_storage.hpp"

Dialog_Template::Dialog_Template()
{
	name = "dialog";

	actor_image = attach<Image>();
	arrow_image = attach<Image>();

	dialog_text	    = attach<Rich_Text>();
	character_name  = attach<Rich_Text>();
	dialog_text_box = attach<Panel>();

	dialog_text->string.set_ascii( "A" );
	character_name->string.set_ascii( "$Dummy text$" );
	actor_image->set_texture_from_name( "portrait_commander" );
	arrow_image->set_texture_from_name( "arrow" );

	dialog_text->character_size    = DIALOG_TEXT_CHAR_SIZE;
	character_name->character_size = CHARACTER_NAME_CHAR_SIZE;

	dialog_text->update_vertices( CHARACTER_LIMIT_PER_LINE );
	character_name->update_vertices();

	character_height = dialog_text->get_global_bounds().size.height;

	constant HALF_PORTRAIT_SIZE   = Size2{ 275.0px / 2, 320.0px / 2 };
	constant HALF_WINDOW_SIZE     = Size2{ 1280.0px / 2, 720.0px / 2 };

	constant ACTOR_IMAGE_POS      = Point{ HALF_WINDOW_SIZE.x - HALF_PORTRAIT_SIZE.x,
										   0.05f * ( HALF_WINDOW_SIZE.y*2 ) };

	constant CHARACTER_NAME_POS   = Point{ HALF_WINDOW_SIZE.x - character_name->get_global_bounds().size.width / 2,
										   ACTOR_IMAGE_POS.y + ( HALF_PORTRAIT_SIZE.y*2 ) * 1.025f };

	constant DIALOG_TEXT_BOX_POS  = Point{ 0.2f * HALF_WINDOW_SIZE.x * 2,
										   CHARACTER_NAME_POS.y + character_name->get_global_bounds().size.height * 1.025f };

	constant DIALOG_TEXT_BOX_SIZE = Size2{ HALF_WINDOW_SIZE.x * 2 - DIALOG_TEXT_BOX_POS.x * 2,
										   HALF_WINDOW_SIZE.y * 0.4f };

	response_text_begin_position  = Point{ DIALOG_TEXT_BOX_POS.x,
										   DIALOG_TEXT_BOX_POS.y + DIALOG_TEXT_BOX_SIZE.y * 1.025f };

	arrow_next_dialog_position    = Point{ DIALOG_TEXT_BOX_POS.x + DIALOG_TEXT_BOX_SIZE.width - arrow_image->get_global_bounds().size.width,
										   DIALOG_TEXT_BOX_POS.y + DIALOG_TEXT_BOX_SIZE.height };


	arrow_choice_response_position= Point{ response_text_begin_position.x - arrow_image->get_global_bounds().size.width * 1.005f, response_text_begin_position.y };

	actor_image->set_global_position( ACTOR_IMAGE_POS );
	character_name->set_global_position( CHARACTER_NAME_POS );
	dialog_text_box->set_global_position( DIALOG_TEXT_BOX_POS );
	dialog_text_box->set_size( DIALOG_TEXT_BOX_SIZE );
	dialog_text_box->set_color( { 255,255,255, 30 } );
	dialog_text->set_global_position( DIALOG_TEXT_BOX_POS );
	arrow_image->set_global_position( arrow_next_dialog_position );

	if ( characters_data is nullptr )
		characters_data = G_Resources_Storage.get_json( "characters" );
}

void Dialog_Template::set_dialog( std::string_view dialog_name )
{
	dialog_json = G_Resources_Storage.get_json( dialog_name.data() );
}

void Dialog_Template::start()
{
	set_up_dialog( "1" );
}

void Dialog_Template::input( sf::Event const& event )
{
	if ( dialog_finished )
		return;

	if ( sf::Event::KeyPressed is event.type ) {
		if ( sf::Keyboard::E is event.key.code ) {
			// @ToDo: Here we can add exit codes for dialogues.
			if ( current_dialog_data.next is "end" ) {
				dialog_finished = true;
				s_dialog_finished.emit();
				return;
			}

			if ( show_responses )
				set_up_dialog( current_dialog_data.responses[selected_response-1].next );
			else
				set_up_dialog( current_dialog_data.next );
		}

		if ( show_responses and current_dialog_data.responses.size() > 1 ) {
			if ( sf::Keyboard::W is event.key.code ) {
				if ( selected_response > 1 )
					selected_response--;
			} else if ( sf::Keyboard::S is event.key.code )
				if ( selected_response < current_dialog_data.responses.size()+1 )
					selected_response++;

			update_arrow_position();
		}
	}
}

void Dialog_Template::add_responses()
{
	for ( auto response : response_text )
		response->queue_for_delete();

	constant responses = current_dialog_data.responses;

	response_text.resize( responses.size() );

	for ( size_t i = 0; i < responses.size(); i++ ) {
		auto& current_text = response_text[i];

		current_text = attach<Rich_Text>();
		current_text->string.set_locale_name( ( *( responses.begin()+i ) ).text );
		current_text->character_size = DIALOG_TEXT_CHAR_SIZE;
		current_text->update_vertices();

		current_text->set_global_position( { response_text_begin_position.x, response_text_begin_position.y + ( character_height* 1.1f )*i } );
	}
}

void Dialog_Template::update_responses_visibility()
{
	for ( auto& text : response_text )
		text->visible = show_responses;
}

void Dialog_Template::update_arrow_position()
{
	if ( show_responses )
		arrow_image->set_global_position( { arrow_choice_response_position.x, arrow_choice_response_position.y + character_height * ( selected_response-1 ) } );
	else
		arrow_image->set_global_position( arrow_next_dialog_position );
}

Dialog_Template::Dialog_Data Dialog_Template::get_dialog_data( std::string const& id )
{
	nlohmann::json data = dialog_json->at( id );
	Dialog_Data data_to_return{ .actor = data.at( "actor" ), .text = data.at( "text" ) };

	if ( auto it = data.find( "responses" ); it is_not data.end() ) {
		for ( auto const& item : it->items() ) {
			data_to_return.responses.emplace_back();
			data_to_return.responses.back().text = item.value().at( "text" );
			data_to_return.responses.back().next = item.value().at( "next" );
		}
	} else
		data_to_return.next = data.at( "next" );

	return data_to_return;
}

void Dialog_Template::set_up_dialog( std::string const& id )
{
	current_dialog_data = get_dialog_data( id );
	auto const character_data = characters_data->at( current_dialog_data.actor );

	actor_image->set_texture_from_name( character_data.at( "portrait" ) );
	character_name->string.set_locale_name( character_data.at( "name" ) );
	character_name->update_vertices();
	dialog_text->string.set_locale_name( current_dialog_data.text );
	dialog_text->update_vertices( CHARACTER_LIMIT_PER_LINE );

	if ( not current_dialog_data.responses.empty() ) {
		selected_response = 1;
		show_responses = true;
		add_responses();
	} else
		show_responses = false;

	update_arrow_position();
	update_responses_visibility();
}
