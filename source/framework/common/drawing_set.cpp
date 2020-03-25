/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "drawing_set.hpp"

#include <execution>

namespace con
{
void Drawing_Set::add_drawable( sf::Drawable& drawable, s32 const layer, sf::RenderStates const render_states )
{
	drawables.emplace_back( Drawable{ layer, &drawable, render_states } );
}

void Drawing_Set::display( sf::RenderTarget& target )
{
	std::sort( std::execution::par_unseq, drawables.begin(), drawables.end(), []( auto const& a, auto const& b ) {
		return a.layer < b.layer;
	} );

	for ( constant& drawable : drawables )
		target.draw( *drawable.drawable, drawable.render_states );
}

Drawing_Set::Drawing_Set()
{
	drawables.reserve( 512 );
}

void Drawing_Set::clear()
{
	drawables.clear();
}
}