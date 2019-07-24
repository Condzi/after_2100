/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"
#include "sprite.hpp"

namespace con
{
// Better don't change anything when animation is playing.
// sprite->visible = false until first play.
class Animation :
	public Node
{
	CLASS_DEF( Animation );

public:
	enum Status
	{
		Playing, Paused, Stopped
	};

	Signal<> s_play;
	Signal<> s_pause;
	Signal<> s_stop;

	Sprite* sprite;

	Animation();

	void set_texture_from_pointer( sf::Texture const* texture );
	void set_texture_from_name( std::string const& name );
	void set_begin_position( Point const& pos );
	void set_frame_size( Size2 const& size );
	void set_frames_count( u32 count );
	void set_duration( r32 sec );
	void set_is_looped( bool val );

	[[nodiscard]] auto get_duration() const      -> r32;
	[[nodiscard]] auto get_eleapsed_time() const -> r32;
	[[nodiscard]] auto get_status() const        -> Status;
	[[nodiscard]] auto is_looped() const         -> bool;

	void play();
	void pause();
	void stop();
	void restart();

	void update( r32 dt ) override;

private:
	Status status{ Stopped };

	bool  looped{ false };
	Point begin_position;
	Size2 frame_size;
	u32   frames_count;
	r32   duration; // in seconds

	r32 time_to_next_frame;
	r32 fps;
	r32 eleapsed;
	u32 current_frame{ 0 };

	void update_fps();
	// Checks if first frame and last frame will fit in bounds of the texture atlas.
	bool is_valid() const;
	void switch_frame();
	void update_frame();
};
}