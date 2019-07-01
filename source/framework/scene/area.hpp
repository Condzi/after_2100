/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "framework/common/shapes.hpp"
#include "framework/common/signal.hpp"
#include "node.hpp"

namespace con
{
class Area :
	public Node
{
	CLASS_DEF( Area );

public:
	// when other area enters
	Signal<Area&> s_area_entered;

	bool      draw_shape{ false };
	sf::Color shape_color{ sf::Color::Red };
	s32       layer{ 0 };

	s32       collision_layer{ 0 };

	Area();
	~Area();

	void set_circle_shape( Circle_Shape const& circle );
	void set_rectangle_shape( Rectangle_Shape const& rectangle );

	[[nodiscard]] auto is_shape_set()	   const -> bool;
	[[nodiscard]] auto get_shape_variant() const -> Shape_Variant const&;

	void draw( Drawing_Set& drawing_set ) override;

private:
	std::variant<sf::RectangleShape, sf::CircleShape> visual_representation;

	bool          shape_set{ false };
	Shape_Variant shape;
};
}