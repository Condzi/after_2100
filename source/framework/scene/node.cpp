/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/debug_flags.hpp"
#include "framework/common/stl_extensions.hpp"
#include "framework/scene/application.hpp"
#include "framework/scene/node.hpp"

namespace con
{
auto Node::attach( Node_Ptr&& node_to_attach ) -> Node* const
{
	G_Profile_Scope( "Node::attach" );

	if ( node_to_attach is nullptr ) {
		engine_log_error( "Given node is empty, can't attach." );
		return nullptr;
	}

	constant is_already_attached = find( child_nodes, node_to_attach ).found;
	if ( is_already_attached ) {
		engine_log_error( "Given node is already attached, can't attach again." );
		return nullptr;
	}

	if ( node_to_attach->parent_node ) {
		engine_log_error( "Given node has a parent already." );
		return nullptr;
	}

	node_to_attach->parent_node = this;
	Node* const return_value = node_to_attach.get();
	child_nodes.push_back( change_owner( node_to_attach ) );

	return return_value;
}

void Node::rotate( r32 deg, bool affect_children )
{
	set_rotation( get_rotation() + deg, affect_children );
	s_rotate.emit( deg );
}

void Node::set_rotation( r32 deg, bool affect_children )
{
	angle = deg;

	if ( affect_children )
		for ( size_t i = 0; i < child_nodes.size(); i++ )
			child_nodes[i]->set_rotation( deg, true );
}

void Node::set_scale( Size2 const& scale_, bool affect_children )
{
	scale = scale_;

	if ( affect_children )
		for ( size_t i = 0; i < child_nodes.size(); i++ )
			child_nodes[i]->set_scale( scale_, true );
}

void Node::apply_scale( Size2 const& scale_, bool affect_children )
{
	set_scale( get_scale() * scale_ );

	if ( affect_children )
		for ( size_t i = 0; i < child_nodes.size(); i++ )
			child_nodes[i]->apply_scale( scale_, true );
}

void Node::set_pause( bool val, bool affect_children )
{
	if ( paused is_not val ) {
		paused = val;
		s_paused.emit( paused );
	}

	if ( affect_children )
		for ( size_t i = 0; i < child_nodes.size(); i++ )
			child_nodes[i]->set_pause( val, true );
}

void Node::queue_for_delete()
{
	s_destroy.emit();

	queued_for_delete = true;

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		child_nodes[i]->queue_for_delete();
}

void Node::bond_disconnector( std::function<void()> disconnector )
{
	auto dc = s_destroy.connect( disconnector );
	// lifetime of connected disconnector is same as s_destroy signal, so it shouldn't make any problems.
	unused( dc );
}

void Node::remove_queued_for_delete()
{
	child_nodes.erase( std::remove_if( child_nodes.begin(), child_nodes.end(),
					   []( constant& child ) { return child->queued_for_delete; } ),
					   child_nodes.end() );

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		child_nodes[i]->remove_queued_for_delete();
}

void Node::update_children( r32 dt )
{
	G_Profile_Scope( get_class_name() );

	if ( not paused ) {
		s_update.emit( dt );
		update( dt );
	}

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		child_nodes[i]->update_children( dt );
}

void Node::handle_input_children( sf::Event const& event )
{
	if ( not paused )
		input( event );

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		child_nodes[i]->handle_input_children( event );
}

void Node::draw_children( Drawing_Set& set )
{
	if ( not paused )
		draw( set );

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		child_nodes[i]->draw_children( set );
}

void Node::draw_gui_children( Drawing_Set& set )
{
	if ( not paused )
		draw_gui( set );

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		child_nodes[i]->draw_gui_children( set );
}

auto Node::get_local_position() const -> Point const&
{
	if ( parent_node is nullptr ) {
		engine_log_warning( "Can't return local position if there is no parent. Returning (0,0)." );
		return {};
	}

	return position - parent_node->position;
}

auto Node::get_parent() -> Node* const
{
	return parent_node;
}

auto Node::get_children() -> std::vector<Node*>
{
	std::vector<Node*> nodes;
	nodes.reserve( child_nodes.size() );

	for ( Node_Ptr& child : child_nodes )
		nodes.emplace_back( child.get() );

	return nodes;
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
		path = path.substr( 5 ); // "root/" has 5 chars, so just cut it off
	}

	std::string name_to_look;
	bool search_for_node{ true };

	auto concatenate_next_name =
		[&]() {
		constant slash_position = path.find( '/' );
		if ( slash_position is std::string::npos ) {
			name_to_look = path;
			search_for_node = false;
			return;
		}

		name_to_look = path.substr( 0, slash_position );
		path = path.substr( slash_position+1 );
	};

	while ( search_for_node ) {
		concatenate_next_name();

		constant[found, idx] = find_if( current_node->child_nodes,
										[&name_to_look]( constant& child ) {
			return child->name == name_to_look;
		} );

		if ( not found )
			return nullptr;

		current_node = current_node->child_nodes[idx].get();
	}

	return current_node;
}

auto Node::get_child_nodes_names() const -> std::vector<std::string>
{
	std::vector<std::string> vec;

	vec.reserve( child_nodes.size() );

	for ( auto& child : child_nodes )
		if ( not child->queued_for_delete )
			vec.emplace_back( child->name );

	return vec;
}

auto Node::get_node( std::string const& path ) -> Node* const
{
	constant node = get_node_or_null( path );

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

auto Node::get_rotation() const -> r32
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
	s_move.emit( vec );

	for ( size_t i = 0; i < child_nodes.size(); i++ )
		if ( child_nodes[i]->move_with_parent )
			child_nodes[i]->move( vec );
}

void Node::set_global_position( Point const& new_position )
{
	constant old_position = position;
	constant offset = new_position - old_position;

	move( offset );
}

void Node::set_absolute_position( Point const& absolute_position, Rectangle_Shape const& rect )
{
	report_error_if( absolute_position.x < -1 or absolute_position.x > 1 or
					 absolute_position.y < -1 or absolute_position.y > 1 )
	{
		return;
	}

	set_global_position( rect.size * absolute_position + rect.position );
}

void Node::set_local_position( Point const& local_position )
{
	report_warning_if( parent_node is nullptr )
	{
		engine_log_warning( "Can't set local position if there is no parent. Returning." );
		return;
	}

	constant old_position = position;
	constant new_position = parent_node->position + local_position;
	constant offset = new_position - old_position;

	move( offset );
}
}