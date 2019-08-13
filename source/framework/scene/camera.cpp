/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "camera.hpp"

namespace con
{
void Camera::set_view( sf::View const& v )
{
	view = v;

	set_center( v.getCenter() );
}

void Camera::set_center( Point const& point )
{
	set_global_position( point );
}

auto Camera::get_center() const -> Point
{
	return view.getCenter();
}

auto Camera::get_view() const -> sf::View
{
	// Meaning that camera is shaking
	if ( shake.trauma > 0 ) {
		sf::View shaked{ view };

		shaked.rotate( shake.angle );
		shaked.move( shake.offset );
		return shaked;
	}
	return view;
}

void Camera::add_shake_trauma( r32 trauma )
{
	shake.trauma = std::min( shake.trauma + trauma, 1.0f );
}

void Camera::stop_shaking()
{
	shake.trauma = 0;
}

void Camera::update( r32 dt )
{
	update_shake( dt );
	update_transformations();
}
void Camera::update_transformations()
{
	view.setRotation( get_rotation() );
	view.setCenter( get_global_position() );
}

void Camera::update_shake( r32 dt )
{
	shake.trauma = std::max( shake.trauma - shake.TRAUMA_DECREASING_RATE * dt, 0.f );

	constant force = shake.trauma * shake.trauma;
	constant random_scalar = [] { return random_real( -1.0f, 1.0f ); };

	shake.angle = shake.MAX_ANGLE * force * random_scalar();
	shake.offset.x = shake.MAX_OFFSET_X * force * random_scalar();
	shake.offset.y = shake.MAX_OFFSET_Y * force * random_scalar();
}
}
