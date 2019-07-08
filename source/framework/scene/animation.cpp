/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "animation.hpp"

namespace con
{
Animation::Animation()
{
	name = "unnamed_animation";

	sprite = attach( Sprite::instantiate() )->cast_to<Sprite>();
	sprite->name = "sprite_to_animate";
}

void Animation::set_texture( sf::Texture const* texture )
{
	if ( texture is nullptr )
		return;

	sprite->set_texture( texture );
}

void Animation::set_begin_position( Point const& pos )
{
	report_warning_if( pos.x < 0 or pos.y < 0 )
	{
		engine_log_warning( "Beggining position of animation frame in texture atlas can't be <0. (value is x={}, y={})", pos.x, pos.y );
		return;
	}

	begin_position = pos;
}

void Animation::set_frame_size( Size2 const& size )
{
	report_warning_if( size.x <= 0 or size.y <= 0 )
	{
		engine_log_warning( "Size of animation frame in texture atlas can't be <=0. (value is x={}, y={})", size.x, size.y );
		return;
	}

	frame_size = size;
}

void Animation::set_frames_count( u32 count )
{
	report_warning_if( count < 1 )
	{
		engine_log_warning( "Number of animation frames can't be < 1. (value is {})", count );
		return;
	}

	frames_count = count;

	update_fps();
}

void Animation::set_duration( r32 sec )
{
	report_warning_if( sec <= 0 )
	{
		engine_log_warning( "Duration of animation can't be <=0s. (value is {}s)", sec );
		return;
	}

	duration = sec;

	update_fps();
}

void Animation::set_is_looped( bool val )
{
	is_looped = val;
}

auto Animation::get_duration() const -> r32
{
	return duration;
}

auto Animation::get_eleapsed_time() const -> r32
{
	return eleapsed;
}

auto Animation::get_status() const -> Status
{
	return status;
}

auto Animation::get_is_looped() const -> bool
{
	return is_looped;
}

void Animation::play()
{
	report_warning_if( status is Status::Playing );

	report_error_if( is_valid() returned false )
	{
		return;
	}

	status = Status::Playing;

	s_play.emit();
}

void Animation::pause()
{
	report_warning_if( status is Status::Paused or status is Status::Stopped );

	status = Status::Paused;

	s_pause.emit();
}

void Animation::stop()
{
	report_warning_if( status is Status::Stopped );

	status = Status::Stopped;

	s_stop.emit();
}

void Animation::restart()
{
	time_to_next_frame = fps;
	eleapsed = 0;
	current_frame = 0;
}

void Animation::update_fps()
{
	if ( frames_count is 0 or duration is 0 ) return;

	fps = duration / frames_count;
}

bool Animation::is_valid() const
{
	constant texture_atlas_bounds = Rectangle_Shape{ {0,0}, sprite->get_texture()->getSize() };
	constant first_frame_pos = begin_position;
	constant last_frame_pos = Vec2{ begin_position.x + frame_size.width * frames_count, begin_position.y };

	constant doesnt_fit_in_atlas = [&]( Vec2 const& point ) {
		return !rect_vs_point( texture_atlas_bounds, point );
	};

	if ( doesnt_fit_in_atlas( first_frame_pos ) or doesnt_fit_in_atlas( first_frame_pos + frame_size ) ) {
		engine_log_error( "First frame of animation doesn't fit in the texture atlas. Frame bounds are: {}, {}, {}, {}. Atlas bounds are: 0, 0, {}, {}", first_frame_pos.x, first_frame_pos.y, frame_size.width, frame_size.height, texture_atlas_bounds.size.width, texture_atlas_bounds.size.height );
		return false;
	}

	if ( doesnt_fit_in_atlas( last_frame_pos ) or doesnt_fit_in_atlas( last_frame_pos + frame_size ) ) {
		engine_log_error( "Last frame of animation doesn't fit in the texture atlas. Frame bounds are: {}, {}, {}, {}. Atlas bounds are: 0, 0, {}, {}", last_frame_pos.x, last_frame_pos.y, frame_size.width, frame_size.height, texture_atlas_bounds.size.width, texture_atlas_bounds.size.height );
		return false;
	}

	return true;
}

void Animation::update( r32 dt )
{
	if ( status is_not Status::Playing )
		return;

	time_to_next_frame -= dt;
	eleapsed += dt;

	if ( time_to_next_frame <= 0 ) {
		switch_frame();

		time_to_next_frame += fps;
	}
}

void Animation::switch_frame()
{
	current_frame++;

	if ( current_frame is frames_count ) {
		if ( is_looped ) 			{
			current_frame = 0;
			eleapsed = 0;
		}
		else
			stop();
	}

	constant frame_pos = Vec2{ begin_position.x + current_frame * frame_size.x, begin_position.y };

	sprite->set_texture_rect( { frame_pos, frame_size } );
}
}