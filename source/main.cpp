/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/scene/node.hpp"
#include "framework/common/debug_log.hpp"
#include "framework/scene/application.hpp"

using namespace con;

class My_Node : public Node
{
	CLASS_DEF( My_Node );

public:
	void update( r32 delta ) override
	{
		calculation_constant x = get_global_position().x;
		calculation_constant y = get_global_position().y;

		log_info( "{}: [{} | {}].", name, x, y );
	}
};

int main()
{
	Application app{ 1280, 720, 60, "after_2100" };

	app.get_root()->attach( My_Node::instantiate() );

	app.run();
}