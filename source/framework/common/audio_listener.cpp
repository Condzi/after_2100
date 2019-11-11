/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "assertions.hpp"
#include "audio_listener.hpp"

#include <SFML/Audio/Listener.hpp>

namespace con::priv
{
void Audio_Listener::set_position( Point const& position )
{
	sf::Listener::setPosition( { position.x, 0, position.y } );
}

void Audio_Listener::set_master_volume( s32 master_volume )
{
	report_error_if( master_volume < 0 or master_volume > 100 )
	{
		return;
	}

	sf::Listener::setGlobalVolume( cast<r32>( master_volume ) );
}

void Audio_Listener::mute()
{
	if ( constant current_volume = sf::Listener::getGlobalVolume(); current_volume is 0 )
		return;
	else
		volume_before_mute = current_volume;

	set_master_volume( 0 );
}

void Audio_Listener::unmute()
{
	if ( sf::Listener::getGlobalVolume() > 0 )
		return;

	set_master_volume( volume_before_mute );
}

Audio_Listener& Audio_Listener::get_instance()
{
	static Audio_Listener* instance = new Audio_Listener;

	return *instance;
}
}