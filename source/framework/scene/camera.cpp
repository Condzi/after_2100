/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "camera.hpp"
#include "framework/scene/application.hpp"

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

void Camera::set_zoom( r32 val )
{
	zoom_val = val;
}

void Camera::zoom( r32 val )
{
	zoom_val += val;
}

auto Camera::get_global_bounds() const -> Rectangle_Shape
{
	Rectangle_Shape bounds;

	bounds.size = get_view().getSize();
	bounds.position.x = get_center().x - bounds.size.width / 2;
	bounds.position.y = get_center().y - bounds.size.height / 2;

	return bounds;
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

auto Camera::get_zoom() const -> r32
{
	return zoom_val;
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
	if ( G_App.is_paused() )
		return;

	Path_Follower::update( dt );

	update_shake( dt );
	update_transformations();
}

void Camera::update_transformations()
{
	view.setRotation( get_rotation() );
	view.setCenter( get_global_position() );
	view.setSize( G_Window.get_size() * zoom_val );
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