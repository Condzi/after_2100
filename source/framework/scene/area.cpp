/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/debug_flags.hpp"
#include "area.hpp"
#include "area_overleaping_checker.hpp"

namespace con
{
Area::Area()
{
	name = "area";

	G_Area_Overleaping_Checker.register_area( this );
}

Area::~Area()
{
	G_Area_Overleaping_Checker.unregister_area( this );
}

void Area::set_circle_shape( Circle_Shape const& circle )
{
	shape_set = true;
	shape = circle;
}

void Area::set_rectangle_shape( Rectangle_Shape const& rectangle )
{
	shape_set = true;
	shape = rectangle;
}

auto Area::is_shape_set() const -> bool
{
	return shape_set;
}

auto Area::get_shape_variant() const -> Shape_Variant const&
{
	return shape;
}

void Area::draw( Drawing_Set& drawing_set )
{
	if ( shape_set is false or G_Debug_Flags.get( "draw_areas" ) is false )
		return;

	std::visit( visitor_overload{
				[&]( Rectangle_Shape const& rectangle ) {
					visual_representation = sf::RectangleShape{};
					auto& rect_to_draw = std::get<sf::RectangleShape>( visual_representation );

					rect_to_draw.setPosition( rectangle.position );
					rect_to_draw.setSize( rectangle.size );
					rect_to_draw.setFillColor( sf::Color::Transparent );
					rect_to_draw.setOutlineThickness( 1.0px );
					rect_to_draw.setOutlineColor( shape_color );
				},
				[&]( Circle_Shape const& circle ) {
					visual_representation = sf::CircleShape{};
					auto& circle_to_draw = std::get<sf::CircleShape>( visual_representation );

					// Center the circle just for comfort.
					circle_to_draw.setOrigin( circle.radious / 2, circle.radious / 2 );
					circle_to_draw.setPosition( circle.center );
					circle_to_draw.setRadius( circle.radious );
					circle_to_draw.setFillColor( sf::Color::Transparent );
					circle_to_draw.setOutlineThickness( 1.0px );
					circle_to_draw.setOutlineColor( shape_color );
				}
				}, shape );

	std::visit(
		[&]( auto&& drawable ) {
			drawing_set.add_drawable( drawable, layer );
		}, visual_representation );
}
}