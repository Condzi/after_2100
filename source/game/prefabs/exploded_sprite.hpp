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
	bool visible{ true };
	s32  layer;
	r32  scale_per_second{ 0.1 }; // how fast the object decreases

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
		std::array<sf::Vertex*, 4> vertices;
	};

	bool initialized{ false };
	sf::Texture const* texture{ nullptr };
	sf::VertexArray        vertices{ sf::Quads, 16 }; // @ToDo: Use std::array when we'll have the ability of rendering vertices directly 
	std::array<Element, 4> elements;
	sf::RenderStates states;
};
