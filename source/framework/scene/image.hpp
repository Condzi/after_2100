/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace con
{
// GUI version of Sprite
class Image : 
	public Node
{
	CLASS_DEF( Image );
	
public:
	s32 layer{ 0 };

	void set_texture( sf::Texture const* texture );
	void set_transformation_origin( Point const& point );
	void set_texture_rect( Rectangle_Shape const& rect );

	[[nodiscard]] auto get_global_bounds() const         -> Rectangle_Shape;
	[[nodiscard]] auto get_texture() const               -> sf::Texture const*;
	[[nodiscard]] auto get_sprite_raw()                  -> sf::Sprite&;
	[[nodiscard]] auto get_transformation_origin() const -> Point;
	[[nodiscard]] auto get_texture_rect()                -> Rectangle_Shape;

	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& set ) override;

private:
	sf::Sprite sprite;
};
}