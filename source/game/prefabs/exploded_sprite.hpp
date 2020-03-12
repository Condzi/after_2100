/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/node.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>

using namespace con;

class Exploded_Sprite :
	public Node
{
	CLASS_DEF( Exploded_Sprite );

public:
	Signal<> s_done_scaling;

	Vec2 initial_velocity{ 0,0 };
	bool visible{ true };
	s32  layer{ 0 };
	r32  scale_per_second{ 0.20 }; // how fast the object decreases
	r32  degress_per_second{ 60.0deg };
	s32  color_variation_range{ 100 }; // makes some parts of elements brighter, other darker
	sf::Color color{ 125, 125, 125 }; // 255 is full color, 0 is black

	Exploded_Sprite();

	void initialize( Vec2 const& max_velocity );
	void explode();

	[[nodiscard]] auto is_exploding() const -> bool;

	void set_texture_from_pointer( sf::Texture const* texture_ );
	void set_texture_from_name( std::string const& name );

	void update( r32 dt ) override;
	void draw( Drawing_Set& set ) override;

private:
	struct Element
	{
		Vec2 velocity;
		sf::RenderStates render_states;
		r32 const random_scalar_for_rotation = random_real( -1, 1 );
		Point center;
		sf::VertexArray vertices{ sf::Quads, 4 };
	};

	bool exploding = false;
	bool elements_initialized{ false };
	bool transformation_initialized{ false };
	sf::Texture const* texture{ nullptr };
	std::array<Element, 4> elements;
};