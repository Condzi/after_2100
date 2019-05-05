/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/scene/node.hpp"
#include "framework/common/debug_log.hpp"
#include "framework/scene/application.hpp"

using namespace con;

int main()
{
	Application::get_instance().initialize( 1280, 720, 60, "after_2100" );
	Application::get_instance().run();
}