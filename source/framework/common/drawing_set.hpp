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
		s32 layer{ 0 };
		sf::Drawable const& drawable;

		bool operator<( Drawable const& second ) const;
		bool operator==( Drawable const& second ) const;
	};

public:
	void clear();
	auto add_drawable( sf::Drawable const& drawable, s32 layer ) -> bool;
	void display( sf::RenderTarget& target );

private:
	std::multiset<Drawable> drawables;
};
}