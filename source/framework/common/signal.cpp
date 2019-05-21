/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "signal.hpp"

#include "framework/scene/application.hpp"

namespace con::priv
{
Signal_Base::Signal_Base()
{
	G_App._add_signal( this );
}

Signal_Base::~Signal_Base()
{
	G_App._remove_signal( this );
}
}