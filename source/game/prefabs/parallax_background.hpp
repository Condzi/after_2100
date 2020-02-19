/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/sprite.hpp"
#include "framework/scene/particle_emitter.hpp"
#include "framework/common/vec2.hpp"

using namespace con;

// Always from right to left.
class Parallax_Background final :
	public Node
{
	CLASS_DEF( Parallax_Background );

public:
	Parallax_Background();

	// Speed to the left.
	r32 speed{ 0 };

	bool visible{ true };
	s32 layer{ 0 };

	void set_texture_from_pointer( sf::Texture const* texture );
	void set_texture_from_name( std::string const& name );

	void update( r32 delta ) override;

private:
	Sprite* sprite_a;
	Sprite* sprite_b;
	Particle_Emitter* stars_particles_a;
	Particle_Emitter* stars_particles_b;
	Particle_Emitter* stars_particles_c;

	void parallax_move( r32 const dt );
	void correct_sprite_positions();
};