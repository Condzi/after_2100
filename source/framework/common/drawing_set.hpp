/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace con
{
class Drawing_Set final
{
	struct Drawable final
	{
		s32           layer{ 0 };
		sf::Drawable* drawable;
		sf::RenderStates render_states{ sf::RenderStates::Default };
	};

public:
	Drawing_Set();

	void clear();
	void add_drawable( sf::Drawable& drawable, s32 const layer, sf::RenderStates const render_states = sf::RenderStates::Default );
	void display( sf::RenderTarget& target );

private:
	std::vector<Drawable> drawables;
};
}