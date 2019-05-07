/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "drawing_set.hpp"

namespace con
{
bool Drawing_Set::Drawable::operator<( Drawable const& second ) const
{
	return layer < second.layer;
}

bool Drawing_Set::Drawable::operator==( Drawable const& second ) const
{
	return &drawable == &second.drawable;
}

bool Drawing_Set::add_drawable( sf::Drawable const& drawable, s32 layer )
{
	return drawables.emplace( Drawable{ layer, drawable } ) != drawables.end();
}

void Drawing_Set::display( sf::RenderTarget& target )
{
	for ( constant& drawable : drawables )
		target.draw( drawable.drawable );
}

void Drawing_Set::clear()
{
	drawables.clear();
}
}
