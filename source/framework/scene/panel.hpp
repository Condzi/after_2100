/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace con
{
class Panel :
	public Node
{
	CLASS_DEF( Panel );

public:
	s32  layer{ 0 };
	bool visible{ true };

	void set_size( Size2 const& size );
	void set_color( sf::Color const& color );

	[[nodiscard]] auto get_global_bounds() const -> Rectangle_Shape;
	[[nodiscard]] auto get_size() const          -> Size2;
	[[nodiscard]] auto get_color() const         -> sf::Color;

	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& set ) override;

private:
	sf::RectangleShape visual;
};
}