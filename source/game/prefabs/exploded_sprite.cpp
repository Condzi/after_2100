/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "exploded_sprite.hpp"

#include "framework/common/resources_storage.hpp"

void Exploded_Sprite::initialize( Vec2 const& max_velocity )
{
	report_warning_if( texture is nullptr )
	{
		return;
	}

	// in percents, <0, 100>
	constant texture_position = [this]( r64 x, r64 y ) {
		return Point{ x,y } *0.01 * Size2{ texture->getSize() };
	};

	// in percents, <0, 100> - in texture space but with offset so it's in correct position
	constant vert_position = [&]( r64 x, r64 y ) {
		return texture_position( x, y ) + get_global_position();
	};

	size_t vertex_idx{ 0 };
	constant next_vertex = [&]() -> sf::Vertex & {
		report_error_if( vertex_idx >= 16 );
		return vertices[vertex_idx++];
	};

	constant initialize_next_vertex = [&]( Point const& pos, Point const& tex ) {
		next_vertex() = sf::Vertex{ pos, tex };
	};

	// 16 of the vertices initialized, 4 quads
	initialize_next_vertex( vert_position( 0,  0 ),  texture_position( 0,  0 ) );
	initialize_next_vertex( vert_position( 50, 0 ),  texture_position( 50, 0 ) );
	initialize_next_vertex( vert_position( 50, 50 ), texture_position( 50, 50 ) );
	initialize_next_vertex( vert_position( 0,  50 ), texture_position( 0,  50 ) );

	initialize_next_vertex( vert_position( 50,  0 ),  texture_position( 50,  0 ) );
	initialize_next_vertex( vert_position( 100, 0 ),  texture_position( 100, 0 ) );
	initialize_next_vertex( vert_position( 100, 50 ), texture_position( 100, 50 ) );
	initialize_next_vertex( vert_position( 0,   50 ), texture_position( 0,   50 ) );

	initialize_next_vertex( vert_position( 0, 50 ),   texture_position( 0,  50 ) );
	initialize_next_vertex( vert_position( 50, 50 ),  texture_position( 50, 50 ) );
	initialize_next_vertex( vert_position( 50, 100 ), texture_position( 50, 100 ) );
	initialize_next_vertex( vert_position( 0, 100 ),  texture_position( 0,  100 ) );

	initialize_next_vertex( vert_position( 50,  50 ),   texture_position( 50,  50 ) );
	initialize_next_vertex( vert_position( 100, 50 ),   texture_position( 100, 50 ) );
	initialize_next_vertex( vert_position( 100, 100 ),  texture_position( 100, 100 ) );
	initialize_next_vertex( vert_position( 50,  100 ),  texture_position( 50,  100 ) );

	constant random_velocity = [&max_velocity] {
		return max_velocity * random_real( -1, 1 );
	};

	for ( size_t i = 0; i < 16; i++ )
		elements[i / 4].vertices[i%4] = &vertices[i];

	for ( auto& element : elements )
		element.velocity = random_velocity();

	initialized = true;
}

void Exploded_Sprite::set_texture_from_pointer( sf::Texture const* texture_ )
{
	texture = texture_;
}

void Exploded_Sprite::set_texture_from_name( std::string const& name )
{
	set_texture_from_pointer( G_Resources_Storage.get_texture( name ) );
}

void Exploded_Sprite::update( r32 dt )
{
	if ( not initialized )
		return;

	transform.scale( Size2{ scale_per_second, scale_per_second } * dt, Size2{ texture->getSize() } * 0.5 + get_global_position() );

	for ( auto& element : elements )
		for ( auto* vertices : element.vertices )
			static_cast<Vec2>( vertices->position ) += element.velocity * dt;
}

void Exploded_Sprite::draw( Drawing_Set& set )
{
	if ( not visible or not initialized )
		return;

	set.add_drawable( vertices, layer, texture );
}

