/*
	Conrad 'Condzi' Kubacki 2019
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

	bool visible{ true };
	s32  layer;
	r32  scale_per_second{ 0.15 }; // how fast the object decreases
	r32  degress_per_second{ 60.0deg };
	sf::Color color{ 125, 125, 125 }; // 255 is full color, 0 is black
	s32 color_variation_range{ 100 }; // makes some parts of elements brighter, other darker

	Exploded_Sprite();

	void initialize( Vec2 const& max_velocity );
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
		r32 const random_scalar_for_scaling = random_real( 0.1, 1 );
		Point center;
		sf::VertexArray vertices{ sf::Quads, 4 };
	};

	bool elements_initialized{ false };
	bool transformation_initialized{ false };
	sf::Texture const* texture{ nullptr };
	std::array<Element, 4> elements;
};
