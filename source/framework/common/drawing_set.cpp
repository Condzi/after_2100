/*
	Conrad 'Condzi' Kubacki 2020
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
	return &drawable is &second.drawable;
}

bool Drawing_Set::add_drawable( sf::Drawable const& drawable, s32 const layer, sf::RenderStates const render_states )
{
	return drawables.emplace( Drawable{ layer, drawable, render_states } ) != drawables.end();
}

void Drawing_Set::display( sf::RenderTarget& target )
{
	for ( constant& drawable : drawables )
		target.draw( drawable.drawable, drawable.render_states );
}

void Drawing_Set::clear()
{
	drawables.clear();
}
}