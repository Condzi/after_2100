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

	[[nodiscard]] auto get_center() const -> Point;
	[[nodiscard]] auto get_view() const   -> sf::View const&;

	void add_shake_trauma( r32 trauma );
	void stop_shaking();

	void update( r32 dt ) override;

private:
	struct
	{
		static constexpr r32 MAX_ANGLE    = 10.0deg;
		static constexpr r32 MAX_OFFSET_X = 100.0px;
		static constexpr r32 MAX_OFFSET_Y = 100.0px;
		static constexpr r32 TRAUMA_DECREASING_RATE = 0.4;

		r32  trauma;
		r32  angle;
		Vec2 offset;
	} shake;

	sf::View view;

	void update_transformations();
	void update_shake( r32 dt );
};
}