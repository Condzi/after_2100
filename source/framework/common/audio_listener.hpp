/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

namespace con::priv
{
class Audio_Listener final
{
public:
	void set_position( Point const& position );
	void set_master_volume( s32 master_volume );

	static Audio_Listener& get_instance();
};
}

#define G_Audio_Listener con::priv::Audio_Listener::get_instance()