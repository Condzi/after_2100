/*
	Conrad 'Condzi' Kubacki 2019
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
		s32 const              layer{ 0 };
		sf::Drawable const& drawable;
		sf::RenderStates const render_states{ sf::RenderStates::Default };

		bool operator<( Drawable const& second ) const;
		bool operator==( Drawable const& second ) const;
	};

public:
	void clear();
	auto add_drawable( sf::Drawable const& drawable, s32 const layer, sf::RenderStates const render_states = sf::RenderStates::Default ) -> bool;
	void display( sf::RenderTarget& target );

private:
	std::multiset<Drawable> drawables;
};
}