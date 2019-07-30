/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace con
{
class Checkbox :
	public Node
{
	CLASS_DEF( Checkbox );

public:
	// bool is the new value
	Signal<bool> s_toggle;

	s32  layer{ 0 };
	bool checked{ false };

	Checkbox();

	void set_size( Size2 const& size );
	void set_fill_color( sf::Color const& color );
	void set_outline_color( sf::Color const& color );
	void set_outline_thickness( r32 thickness );

	[[nodiscard]] auto get_size() const                 -> Size2;
	[[nodiscard]] auto get_fill_color_checked() const   -> sf::Color;
	[[nodiscard]] auto get_fill_color_unchecked() const -> sf::Color;
	[[nodiscard]] auto get_outline_color() const        -> sf::Color;
	[[nodiscard]] auto get_outline_thickness() const    -> r32;

	void input( sf::Event const& event ) override;
	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& set ) override;

private:
	sf::Color fill_checked;
	sf::Color fill_unchecked;
	sf::RectangleShape visual;
};
}