/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/scene/node.hpp"
#include "framework/common/debug_log.hpp"

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
	My_Node* node_1{ nullptr };
	My_Node* node_2{ nullptr };
	My_Node* node_3{ nullptr };
	// Parent node has to stay alive whole time.
	auto n_1 = My_Node::instantiate();

	{
		auto n_2 = My_Node::instantiate();
		auto n_3 = My_Node::instantiate();

		n_1->name = "first";
		n_2->name = "second";
		n_3->name = "third";

		node_3 = dynamic_cast<My_Node*>( n_2->attach( change_owner( n_3 ) ) );
		node_2 = dynamic_cast<My_Node*>( n_1->attach( change_owner( n_2 ) ) );
		node_1 = n_1.get();
	}

	auto log_message = [&]() {
		log_info( "====================" );
		node_1->update_children( 0 );
	};

	log_message();

	node_1->move( { 20, 30 } );
	log_message();

	node_2->move( { 10, 10 } );
	log_message();

	node_3->move( { 1,1 } );
	log_message();

	node_1->set_global_position( { 0,0 } );
	log_message();

	node_2->set_local_position( { 1,1 } );
	log_message();

	node_3->set_global_position( { 5,5 } );
	log_message();

	node_2->set_pause( true );
	log_message();
	log_info( "(paused)" );

	node_1->move( { 100,100 } );
	log_message();
	log_info( "(paused)" );

	node_2->set_pause( false );
	log_message();
	log_info( "(unpaused)" );

	node_1->get_node( "second/third" )->name = "better third";
	log_message();

	node_2->get_node( "better third" )->name = "another third bites the dust";
	log_message();

	node_2->queue_for_delete();
	node_1->remove_queued_for_delete();
	log_message();

	node_1->get_node( "I don't exist! :(" );
}