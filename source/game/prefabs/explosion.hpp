/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/common/resources_storage.hpp"
#include "framework/scene/particle_emitter.hpp"
#include "framework/scene/sound_source.hpp"

using namespace con;

class Explosion :
	public Node
{
	CLASS_DEF( Explosion );

public:
	s32 layer;

	Explosion();

	void setup();
	void explode();

	Particle_Emitter* particles_a;
	Particle_Emitter* particles_b;
	Particle_Emitter* particles_c;
	Particle_Emitter* particles_d;

	Sound_Source* sound;

private:
	bool is_ready{ false };
};