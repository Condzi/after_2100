/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "dialog_template.hpp"

Dialog_Template::Dialog_Template()
{
	name = "dialog";

	actor_image = attach<Image>();
	arrow_image = attach<Image>();

	dialog_text	    = attach<Rich_Text>();
	character_name  = attach<Rich_Text>();
	dialog_text_box = attach<Panel>();

	dialog_text->string.set_locale_name( "#loc_prolog_1" );
	character_name->string.set_ascii( "$Dummy text$" );
	actor_image->set_texture_from_name( "portrait_commander" );
	arrow_image->set_texture_from_name( "arrow" );

	dialog_text->character_size    = 32;
	character_name->character_size = 20;

	dialog_text->update_vertices( 50 );
	character_name->update_vertices();

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

	constant ARROW_POSITION       = Point{ DIALOG_TEXT_BOX_POS.x + DIALOG_TEXT_BOX_SIZE.width - arrow_image->get_global_bounds().size.width,
										   DIALOG_TEXT_BOX_POS.y + DIALOG_TEXT_BOX_SIZE.height };

	response_text_begin_position  = Point{ DIALOG_TEXT_BOX_POS.x,
										   DIALOG_TEXT_BOX_POS.y + DIALOG_TEXT_BOX_SIZE.y * 1.025f };

	actor_image->set_global_position( ACTOR_IMAGE_POS );
	character_name->set_global_position( CHARACTER_NAME_POS );
	dialog_text_box->set_global_position( DIALOG_TEXT_BOX_POS );
	dialog_text_box->set_size( DIALOG_TEXT_BOX_SIZE );
	dialog_text_box->set_color( { 255,255,255, 10 } );
	dialog_text->set_global_position( DIALOG_TEXT_BOX_POS );
	arrow_image->set_global_position( ARROW_POSITION );
}

void Dialog_Template::update_responses_visibility()
{
	for ( auto& text : response_text )
		text->visible = show_responses;
}
