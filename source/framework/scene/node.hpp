/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "framework/common/vec2.hpp"

namespace sf
{
class Event;
class RenderWindow;
}

namespace con
{
class Node
{
	CLASS_DEF( Node );

	friend class Application;

public:
	using Node_Ptr = std::unique_ptr<Node>;

	std::string name{ get_class_name() };

	virtual ~Node() = default;

	[[nodiscard]] auto attach( Node_Ptr&& node_to_attach )  -> Node *const;
	[[nodiscard]] auto get_parent()                         -> Node *const;
	[[nodiscard]] auto get_node_or_null( std::string path ) -> Node *const;
	[[nodiscard]] auto get_node( std::string const& path )  -> Node *const;

	[[nodiscard]] auto get_global_position() const          -> Point const&;
	[[nodiscard]] auto get_local_position() const           -> Point const&;
	[[nodiscard]] auto get_rotaton() const                  -> r32;
	[[nodiscard]] auto get_scale() const                    -> Size2 const&;

	[[nodiscard]] auto is_paused() const                    -> bool;
	[[nodiscard]] auto is_queued_for_delete() const         -> bool;

	void move( Vec2 const& vec );
	void set_local_position( Point const& local_position );
	void set_global_position( Point const& new_position );

	void rotate( r32 deg, bool affect_children = true );
	void set_rotation( r32 deg, bool affect_children = true );

	void set_scale( Size2 const& scale_, bool affect_children = true );
	void apply_scale( Size2 const& scale_, bool affect_children = true );

	void set_pause( bool val, bool affect_children = true );
	void queue_for_delete();

	virtual void update( r32 delta ) {}
	virtual void handle_input( sf::Event const& event ) {}
	virtual void draw( sf::RenderWindow& window ) {}

private:
	Node* parent_node{ nullptr }; // The most outside nodes don't have parents.
	std::vector<Node_Ptr> child_nodes;

	Point position{ 0, 0 };
	Size2 scale{ 1, 1 };
	r32 angle{ 0 };

	bool queued_for_delete{ false };
	bool paused{ false };

	// @ToDo: public is debug only
public:
	void remove_queued_for_delete();
	void update_children( r32 dt );
	void handle_input_children( sf::Event const& event );
	void draw_children( sf::RenderWindow& window );
};
}
