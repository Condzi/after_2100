/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "framework/common/assertions.hpp"
#include "framework/common/vec2.hpp"
#include "framework/common/drawing_set.hpp"
#include "framework/common/signal.hpp"
#include "framework/common/shapes.hpp"
#include "framework/common/window.hpp"

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

	Signal<>     s_destroy;
	Signal<Vec2> s_move; //< Vec2 is the offset.
	Signal<r32>  s_update; //< r32 is delta time
	Signal<r32>  s_rotate; //< r32 is how many degrees was added
	Signal<Drawing_Set&> s_draw;

	std::string name{ "unnamed_node" };
	bool        move_with_parent{ true };

	Node();
	virtual ~Node() = default;

	template <typename TNode>
	auto attach()                             -> TNode* const;
	auto attach( Node_Ptr&& node_to_attach )  -> Node* const;

	template <typename TNode>
	[[nodiscard]] auto cast_to()                            -> TNode* const;
	[[nodiscard]] auto get_parent()                         -> Node* const;
	[[nodiscard]] auto get_node_or_null( std::string path ) -> Node* const;
	[[nodiscard]] auto get_node( std::string const& path )  -> Node* const;

	[[nodiscard]] auto get_global_position() const          -> Point const&;
	[[nodiscard]] auto get_local_position() const           -> Point const&;
	[[nodiscard]] auto get_rotation() const                 -> r32;
	[[nodiscard]] auto get_scale() const                    -> Size2 const&;

	[[nodiscard]] auto is_paused() const                    -> bool;
	[[nodiscard]] auto is_queued_for_delete() const         -> bool;

	void move( Vec2 const& vec );
	void set_local_position( Point const& local_position );
	void set_global_position( Point const& new_position );
	// if no rect is given then {0,0, win_size.x, win_size.y} is used.
	void set_absolute_position( Point const& absolute_position, Rectangle_Shape const& rect = G_Window.get_bounds() );

	void rotate( r32 deg, bool affect_children = true );
	void set_rotation( r32 deg, bool affect_children = true );

	void set_scale( Size2 const& scale_, bool affect_children = true );
	void apply_scale( Size2 const& scale_, bool affect_children = true );

	void set_pause( bool val, bool affect_children = true );
	void queue_for_delete();

	// Example:
	// auto disconnector = signal.connect([]{...});
	// node.bond_disconnector(disconnector);
	//
	// Use it when function connected to the signal will be invalidated
	// after destruction of the node.
	void bond_disconnector( std::function<void()> disconnector );

	virtual void update( r32 delta ) {}
	virtual void input( sf::Event const& event ) {}
	// Use it for drawing own stuff, but prefer Sprite (or Text) node.
	virtual void draw( Drawing_Set& drawing_set ) {}
	virtual void draw_gui( Drawing_Set& drawing_set ) {}

private:
	std::vector<Node_Ptr> child_nodes;
	Node* parent_node{ nullptr }; // The most outside nodes don't have parents.

	Point position{ 0.0px, 0.0px };
	Size2 scale{ 1, 1 };
	r32   angle{ 0.0deg };

	sf::CircleShape debug_position_visual;

	bool queued_for_delete{ false };
	bool paused{ false };

	void remove_queued_for_delete();
	void update_children( r32 dt );
	void handle_input_children( sf::Event const& event );
	void draw_children( Drawing_Set& set );
	void draw_gui_children( Drawing_Set& set );
};
}

#include "node.inl"