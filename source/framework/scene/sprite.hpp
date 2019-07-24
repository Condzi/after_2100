/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/common/sprite3d.hpp"

#include "node.hpp"
#include "framework/common/shapes.hpp"

namespace con
{
class Sprite :
	public Node
{
	CLASS_DEF( Sprite );

	struct Rotation3D final
	{
		r32 pitch{ 0 }, yaw{ 0 }, roll{ 0 };
	};

public:
	bool visible{ true };
	s32  layer{ 0 };

	void set_texture_from_pointer( sf::Texture const* texture );
	void set_texture_from_name( std::string const& name );
	void set_rotation_3d( r32 pitch, r32 yaw, r32 roll );
	void set_transformation_origin( Point const& point );
	void set_texture_rect( Rectangle_Shape const& rect );

	[[nodiscard]] auto get_global_bounds() const         -> Rectangle_Shape;
	[[nodiscard]] auto get_texture() const               -> sf::Texture const*;
	[[nodiscard]] auto get_rotation_3d() const           -> Rotation3D;
	[[nodiscard]] auto get_sprite_raw()                  -> sf::Sprite3d &;
	[[nodiscard]] auto get_transformation_origin() const -> Point;
	[[nodiscard]] auto get_texture_rect()                -> Rectangle_Shape;

	void update( r32 delta ) override;
	void draw( Drawing_Set& drawing_set ) override;

private:
	sf::Sprite3d sprite;

};
}