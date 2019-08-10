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
	size_t element_idx{ 0 };
	constant next_vertex = [&]() -> sf::Vertex & {
		if ( vertex_idx is 4 ) {
			vertex_idx = 0;
			++element_idx;
		}
		return elements[element_idx].vertices[vertex_idx++];
	};

	constant initialize_next_vertex = [&]( Point const& pos, Point const& tex ) {
		constant color_variation_value = random_int( -color_variation_range, color_variation_range );
		constant color_variation = sf::Color( color.r + color_variation_value, color.g + color_variation_value, color.b + color_variation_value );

		next_vertex() = sf::Vertex{ pos, color_variation, tex };
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
		return Vec2{ max_velocity } *Vec2{ random_real( 0.2, 1 ), random_real( -1, 1 ) };
	};


	for ( auto& element : elements ) {
		element.velocity = random_velocity();
		element.center = Vec2{ texture->getSize() } *0.25;
	}

	elements_initialized = true;
}

Exploded_Sprite::Exploded_Sprite()
{
	bond_disconnector( s_move.connect( [this]( Vec2 const& offset ) {
		for ( auto& element : elements )
			for ( size_t i = 0; i < 4; i++ ) {
				auto& vertex = element.vertices[i];
				vertex.position += static_cast<sf::Vector2f>( offset );
			}
					   } ) );
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
	if ( elements_initialized is false )
		return;

	if ( get_scale().x >= 0.01 and get_scale().y >= 0.01 )
		set_scale( get_scale() - Vec2{ scale_per_second, scale_per_second } *dt );
	else {
		s_done_scaling.emit();
		elements_initialized = false;
		return;
	}

	rotate( degress_per_second * dt );

	for ( auto& element : elements ) {
		for ( size_t i = 0; i < 4; i++ ) {
			auto& vertex = element.vertices[i];
			vertex.position += static_cast<sf::Vector2f>( element.velocity * dt );
		}

		element.render_states.transform = sf::Transform::Identity;
		element.render_states.transform.rotate( get_rotation() * element.random_scalar_for_rotation, element.center + element.vertices[0].position );
		element.render_states.transform.scale( get_scale(), element.center + element.vertices[0].position );
	}

	if ( not transformation_initialized )
		transformation_initialized = true;
}

void Exploded_Sprite::draw( Drawing_Set& set )
{
	if ( not visible or not transformation_initialized )
		return;

	for ( auto& element : elements ) {
		element.render_states.texture = texture;
		set.add_drawable( element.vertices, layer, element.render_states );
	}
}