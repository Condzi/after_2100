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
	initialize_next_vertex( vert_position( 0, 0 ), texture_position( 0, 0 ) );
	initialize_next_vertex( vert_position( 50, 0 ), texture_position( 50, 0 ) );
	initialize_next_vertex( vert_position( 50, 50 ), texture_position( 50, 50 ) );
	initialize_next_vertex( vert_position( 0, 50 ), texture_position( 0, 50 ) );

	initialize_next_vertex( vert_position( 50, 0 ), texture_position( 50, 0 ) );
	initialize_next_vertex( vert_position( 100, 0 ), texture_position( 100, 0 ) );
	initialize_next_vertex( vert_position( 100, 50 ), texture_position( 100, 50 ) );
	initialize_next_vertex( vert_position( 0, 50 ), texture_position( 0, 50 ) );

	initialize_next_vertex( vert_position( 0, 50 ), texture_position( 0, 50 ) );
	initialize_next_vertex( vert_position( 50, 50 ), texture_position( 50, 50 ) );
	initialize_next_vertex( vert_position( 50, 100 ), texture_position( 50, 100 ) );
	initialize_next_vertex( vert_position( 0, 100 ), texture_position( 0, 100 ) );

	initialize_next_vertex( vert_position( 50, 50 ), texture_position( 50, 50 ) );
	initialize_next_vertex( vert_position( 100, 50 ), texture_position( 100, 50 ) );
	initialize_next_vertex( vert_position( 100, 100 ), texture_position( 100, 100 ) );
	initialize_next_vertex( vert_position( 50, 100 ), texture_position( 50, 100 ) );

	constant random_velocity = [&max_velocity] {
		return Vec2{ max_velocity } *Vec2{ random_real( -1, 1 ), random_real( -1, 1 ) };
	};

	for ( size_t i = 0; i < 16; i++ )
		elements[i / 4].vertices[i%4] = &vertices[i];

	for ( auto& element : elements )
		element.velocity = random_velocity();

	initialized = true;
}

Exploded_Sprite::Exploded_Sprite()
{
	bond_disconnector( s_move.connect( [this]( Vec2 const& offset ) {
		for ( size_t i = 0; i < vertices.getVertexCount(); i++ )
			vertices[i].position += static_cast<sf::Vector2f>( offset );
					   } ) );
}

void Exploded_Sprite::set_texture_from_pointer( sf::Texture const* texture_ )
{
	texture = texture_;
	states.texture = texture;
}

void Exploded_Sprite::set_texture_from_name( std::string const& name )
{
	set_texture_from_pointer( G_Resources_Storage.get_texture( name ) );
}

void Exploded_Sprite::update( r32 dt )
{
	if ( not initialized )
		return;

	if ( get_scale().x >= 0.01 and get_scale().y >= 0.01 )
		set_scale( get_scale() - Vec2{ scale_per_second, scale_per_second } *dt );

	for ( auto& element : elements )
		for ( auto* vertices : element.vertices )
			vertices->position += static_cast<sf::Vector2f>( element.velocity * dt );
}

void Exploded_Sprite::draw( Drawing_Set& set )
{
	if ( not visible or not initialized )
		return;

	constant center = Vec2{ texture->getSize().y, texture->getSize().x } *0.25f;
	states.transform = sf::Transform::Identity;
	states.transform.rotate( get_rotation(), center );
	states.transform.scale( get_scale(), center );

	set.add_drawable( vertices, layer, states );
}