/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "framework/common/vec2.hpp"
#include "framework/common/drawing_set.hpp"

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

	std::string name{ "unnamed_node" };

	virtual ~Node() = default;

	template <typename TNode>
	[[nodiscard]] auto cast_to()                            -> TNode *const;

	auto attach( Node_Ptr&& node_to_attach )                -> Node *const;
	[[nodiscard]] auto get_parent()                         -> Node *const;
	[[nodiscard]] auto get_node_or_null( std::string path ) -> Node *const;
	[[nodiscard]] auto get_node( std::string const& path )  -> Node *const;

	[[nodiscard]] auto get_global_position() const          -> Point const&;
	[[nodiscard]] auto get_local_position() const           -> Point const&;
	[[nodiscard]] auto get_rotation() const                 -> r32;
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
	virtual void input( sf::Event const& event ) {}
	// Use it for drawing own stuff, but prefer Sprite (or Text) node.
	virtual void draw( Drawing_Set& drawing_set ) {}

private:
	std::vector<Node_Ptr> child_nodes;
	Node*                 parent_node{ nullptr }; // The most outside nodes don't have parents.

	Point position{ 0.0px, 0.0px };
	Size2 scale{ 1, 1 };
	r32   angle{ 0.0deg };

	bool queued_for_delete{ false };
	bool paused{ false };

	void remove_queued_for_delete();
	void update_children( r32 dt );
	void handle_input_children( sf::Event const& event );
	void draw_children( Drawing_Set& queue );
};
}

#include "node.inl"