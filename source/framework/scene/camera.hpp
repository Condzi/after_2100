/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once


#include <SFML/Graphics/View.hpp>

#include "path.hpp"
#include "framework/common/vec2.hpp"
#include "framework/common/random.hpp"

namespace con
{

// Deriving from Path_Follower allows for making better
// cut scenes. Just make a path and make camera follow it.
class Camera :
	public Path_Follower
{
	CLASS_DEF( Camera );

public:
	void set_view( sf::View const& v );
	void set_center( Point const& point );
	void set_zoom( r32 val );
	void zoom( r32 val );

	[[nodiscard]] auto get_center() const -> Point;
	[[nodiscard]] auto get_view() const   -> sf::View;
	[[nodiscard]] auto get_zoom() const   -> r32;

	void add_shake_trauma( r32 trauma );
	void stop_shaking();

	void update( r32 dt ) override;

private:
	struct
	{
		compile_constant MAX_ANGLE = 10.0deg;
		compile_constant MAX_OFFSET_X = 100.0px;
		compile_constant MAX_OFFSET_Y = 100.0px;
		compile_constant TRAUMA_DECREASING_RATE = 0.3f;

		r32  trauma{ 0 };
		r32  angle{ 0 };
		Vec2 offset{ 0,0 };
	} shake;

	sf::View view;
	r32 zoom_val = 1.0;

	void update_transformations();
	void update_shake( r32 dt );
};
}