/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"
#include "animation.hpp"

namespace con
{
Animation::Animation()
{
	name = "unnamed_animation";

	sprite = attach<Sprite>();
	sprite->name = "sprite_to_animate";
	sprite->visible = false;
}

void Animation::set_texture_from_pointer( sf::Texture const* texture )
{
	sprite->set_texture_from_pointer( texture );
}

void Animation::set_texture_from_name( std::string const& name )
{
	set_texture_from_pointer( G_Resources_Storage.get_texture( name ) );
}

void Animation::set_begin_position( Point const& pos )
{
	if( pos.x < 0 or pos.y < 0 )
	{
		engine_log_warning( "Beggining position of animation frame in texture atlas can't be <0. (value is x={}, y={})", pos.x, pos.y );
		return;
	}

	begin_position = pos;
}

void Animation::set_frame_size( Size2 const& size )
{
	if( size.x <= 0 or size.y <= 0 )
	{
		engine_log_warning( "Size of animation frame in texture atlas can't be <=0. (value is x={}, y={})", size.x, size.y );
		return;
	}

	frame_size = size;
}

void Animation::set_frames_count( u32 count )
{
	if( count < 1 )
	{
		engine_log_warning( "Number of animation frames can't be < 1. (value is {})", count );
		return;
	}

	frames_count = count;

	update_fps();
}

void Animation::set_duration( r32 sec )
{
	if( sec <= 0 )
	{
		engine_log_warning( "Duration of animation can't be <=0s. (value is {}s)", sec );
		return;
	}

	duration = sec;

	update_fps();
}

void Animation::set_is_looped( bool val )
{
	looped = val;
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

auto Animation::is_looped() const -> bool
{
	return looped;
}

void Animation::play()
{
	report_warning_if( status is Status::Playing );

	report_error_if( is_valid() returned false )
	{
		return;
	}

	status = Status::Playing;

	sprite->visible = true;
	update_frame();
	
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
	constant atlas_size = Vec2{ sprite->get_texture()->getSize() };
	constant first_frame_pos = begin_position;
	constant last_frame_pos = Vec2{ begin_position.x + frame_size.width * (frames_count-1), begin_position.y };

	constant doesnt_fit_in_atlas = [&]( Vec2 const& point ) {
		return point.x < 0 or point.x > atlas_size.x or
			point.y < 0 or point.y > atlas_size.y;
	};

	if ( doesnt_fit_in_atlas( first_frame_pos ) or doesnt_fit_in_atlas( first_frame_pos + frame_size ) ) {
		engine_log_error( "First frame of animation doesn't fit in the texture atlas. Frame bounds are: {}, {}, {}, {}. Atlas bounds are: 0, 0, {}, {}", first_frame_pos.x, first_frame_pos.y, frame_size.width, frame_size.height, atlas_size.width, atlas_size.height );
		return false;
	}

	if ( doesnt_fit_in_atlas( last_frame_pos ) or doesnt_fit_in_atlas( last_frame_pos + frame_size ) ) {
		engine_log_error( "Last frame of animation doesn't fit in the texture atlas. Frame bounds are: {}, {}, {}, {}. Atlas bounds are: 0, 0, {}, {}", last_frame_pos.x, last_frame_pos.y, frame_size.width, frame_size.height, atlas_size.width, atlas_size.height);
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
		if ( looped ) 			{
			current_frame = 0;
			eleapsed = 0;
		}
		else
			stop();
	}

	update_frame();
}

void Animation::update_frame()
{
	constant frame_pos = Vec2{ begin_position.x + current_frame * frame_size.x, begin_position.y };

	sprite->set_texture_rect( { frame_pos, frame_size } );
}
}