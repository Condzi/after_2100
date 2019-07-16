/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/Graphics/Text.hpp>

#include "node.hpp"
#include "framework/common/shapes.hpp"

namespace con
{
class Label final :
	public Node
{
	CLASS_DEF( Label );

public:
	s32 layer;

	Label();

	// for example "#loc1" 
	void set_locale( std::string const& name );
	void set_font( std::string const& name );
	void set_character_size( size_t size );
	void set_fill_color( sf::Color const& color );

	[[nodiscard]] auto get_locale() const         -> std::string const&;
	[[nodiscard]] auto get_character_size() const -> size_t;
	[[nodiscard]] auto get_global_bounds() const  -> Rectangle_Shape;

	void draw_gui( Drawing_Set& set ) override;

	void update( r32 dt) override;

private:
	sf::Text text;
	std::string locale_name;
	sf::String const* locale_string;
};
}