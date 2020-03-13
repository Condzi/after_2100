/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/clickable_label.hpp"

using namespace con;

class Main_Menu final :
	public Node
{
	CLASS_DEF( Main_Menu );

public:
	Main_Menu();

private:
	Rich_Text* text_title;
	Clickable_Label* label_new_game;
	Clickable_Label* label_exit;
	Rich_Text* text_language;
	Clickable_Label* label_language_selection;
};