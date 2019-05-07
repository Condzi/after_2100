/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "drawing_set.hpp"

namespace con
{
bool Drawing_Queue::Drawable::operator<( Drawable const& second ) const
{
	return layer < second.layer;
}

bool Drawing_Queue::Drawable::operator==( Drawable const& second ) const
{
	return &drawable == &second.drawable;
}

bool Drawing_Queue::add_drawable( sf::Drawable const& drawable, s32 layer )
{
	return drawables.insert( Drawable{ layer, drawable } ).second;
}

void Drawing_Queue::display( sf::RenderTarget& target )
{
	for ( constant& drawable : drawables )
		target.draw( drawable.drawable );
}

void Drawing_Queue::clear()
{
	drawables.clear();
}
}
