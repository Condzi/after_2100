/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "node.hpp"
#include "framework/common/shapes.hpp"

namespace con
{
class Sprite :
	public Node
{
	CLASS_DEF( Sprite )

public:
	bool visible{ true };
	s32 layer{ 0 };

	void update( r32 delta ) override;
	void draw( Drawing_Set& drawing_set ) override;

	void set_texture( sf::Texture const* texture );

	[[nodiscard]] auto get_global_bounds() const -> Rectangle_Shape;
	[[nodiscard]] auto get_texture() const       -> sf::Texture const*;

private:
	sf::Sprite sprite;
};
}