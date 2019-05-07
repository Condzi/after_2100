/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/node.hpp"
#include "framework/common/assertions.hpp"
#include "framework/common/stl_extensions.hpp"
#include "framework/scene/application.hpp"

namespace con
{
void Node::rotate( r32 deg, bool affect_children )
{
	set_rotation( get_rotaton() + deg, affect_children );
}

void Node::set_rotation( r32 deg, bool affect_children )
{
	angle = deg - 360 * ( static_cast<s32>( deg ) % 360 );

	if ( affect_children )
		for ( auto& child : child_nodes )
			child->set_rotation( deg, true );
}

void Node::set_scale( Size2 const& scale_, bool affect_children )
{
	scale = scale_;

	if ( affect_children )
		for ( auto& child : child_nodes )
			child->set_scale( scale_, true );
}

void Node::apply_scale( Size2 const& scale_, bool affect_children )
{
	set_scale( get_scale() * scale_ );

	if ( affect_children )
		for ( auto& child : child_nodes )
			child->apply_scale( scale_, true );
}

void Node::set_pause( bool val, bool affect_children )
{
	paused = val;

	if ( affect_children )
		for ( auto& child : child_nodes )
			child->set_pause( val, true );
}

void Node::queue_for_delete()
{
	queued_for_delete = true;
}

void Node::remove_queued_for_delete()
{
	child_nodes.erase( std::remove_if( child_nodes.begin(), child_nodes.end(),
					   []( auto const& child ) { return child->queued_for_delete; } ),
					   child_nodes.end() );

	for ( auto& child : child_nodes )
		child->remove_queued_for_delete();
}

void Node::update_children( r32 dt )
{
	if ( not paused )
		update( dt );

	for ( auto& child : child_nodes )
		child->update_children( dt );
}

void Node::handle_input_children( sf::Event const& event )
{
	if ( not paused )
		handle_input( event );

	for ( auto& child : child_nodes )
		child->handle_input_children( event );
}

void Node::draw_children( Drawing_Queue& queue )
{
	draw( queue );

	for ( auto& child : child_nodes )
		child->draw_children( queue );
}

auto Node::get_local_position() const -> Point const&
{
	report_warning_if( parent_node is nullptr )
	{
		engine_log_warning( "Can't return local position if there is no parent. Returning (0,0)." );
		return {};
	}

	return position - parent_node->position;
}

auto Node::attach( Node_Ptr&& node_to_attach ) -> Node *const
{
	report_error_if( node_to_attach is nullptr )
	{
		engine_log_error( "Given node is empty, can't attach." );
		return nullptr;
	}

	calculation_constant is_already_attached = find( child_nodes, node_to_attach ).found;
	report_error_if( is_already_attached )
	{
		engine_log_error( "Given node is already attached, can't attach again." );
		return nullptr;
	}

	report_error_if( node_to_attach->parent_node )
	{
		engine_log_error( "Given node has a parent already." );
		return nullptr;
	}

	node_to_attach->parent_node = this;
	Node *const return_value = node_to_attach.get();
	child_nodes.emplace_back() = change_owner( node_to_attach );

	return return_value;
}

auto Node::get_parent() -> Node *const
{
	return parent_node;
}

auto Node::get_node_or_null( std::string path ) -> Node* const
{
	report_error_if( path.empty() )
	{
		return nullptr;
	}

	Node* current_node{ this };

	if ( string_begins_with( path, "root/" ) ) {
		current_node = &G_Root;
		path = path.substr( 6 ); // "root/" has 6 chars, so just cut it off
	}

	std::string name_to_look;
	bool search_for_node{ true };

	auto concatenate_next_name = [&]() {
		calculation_constant slash_position = path.find( '/' );
		if ( slash_position == std::string::npos ) {
			name_to_look = path;
			search_for_node = false;
			return;
		}

		name_to_look = path.substr( 0, slash_position );
		path = path.substr( slash_position+1 );
	};

	while ( search_for_node ) {
		concatenate_next_name();

		calculation_constant[found, idx] = find_if( current_node->child_nodes,
													[&name_to_look]( auto const& child ) {
			return child->name == name_to_look;
		} );

		if ( not found )
			return nullptr;

		current_node = current_node->child_nodes[idx].get();
	}

	return current_node;
}

auto Node::get_node( std::string const& path ) -> Node* const
{
	calculation_constant node = get_node_or_null( path );

	report_error_if( node is nullptr )
	{
		engine_log_error( "Can't find node \"{}\" in node \"{}\".", path, name );
	}

	return node;
}

auto Node::get_global_position() const -> Point const&
{
	return position;
}

auto Node::get_rotaton() const -> r32
{
	return angle;
}

auto Node::is_paused() const -> bool
{
	return paused;
}

auto Node::is_queued_for_delete() const -> bool
{
	return queued_for_delete;
}

auto Node::get_scale() const -> Size2 const&
{
	return scale;
}

void Node::move( Vec2 const& vec )
{
	position += vec;

	for ( auto& child : child_nodes )
		child->move( vec );
}

void Node::set_global_position( Point const& new_position )
{
	calculation_constant old_position = position;
	calculation_constant offset = new_position - old_position;

	move( offset );
}

void Node::set_local_position( Point const& local_position )
{
	report_warning_if( parent_node is nullptr )
	{
		engine_log_warning( "Can't set local position if there is no parent. Returning." );
		return;
	}

	calculation_constant old_position = position;
	calculation_constant new_position = parent_node->position + local_position;
	calculation_constant offset = new_position - old_position;

	move( offset );
}
}