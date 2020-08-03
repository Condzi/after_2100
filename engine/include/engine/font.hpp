#pragma once

#include <engine/array.hpp>
#include <engine/text_size.hpp>
#include <engine/hashed_cstring.hpp>
#include <engine/utf8_string.hpp>

#include <schrift/schrift.h>

namespace con
{
// Basically just a container for character texture atlas.
class Font final
{
public:
	struct Character_Info final
	{
		// Offset from the baseline.
		v2 pen_offset{ -1, -1 };

		s16 width  = -1;
		s16 height = -1;
		// Since texture atlas is just one long line, this is 
		// and horizontal offset. Vertical doesn't exist.
		s16 offset_in_texture = -1;
	};

	// Every font should know what sizes it needs.
	// For example developer console font (Cascadia Code right now)
	// has only one size, but other fonts may have to have more than 
	// one size.
	//
	// text_sizes should be an initializer list of Text_Size::X enum.
	void initialize( CString path, std::initializer_list<s8> text_sizes );
	void shutdown();

	// we'll need a function like that in the future.
	// void update_textures_to_new_sizes();

	// Text size should be Text_Size::X enum.
	returning get_texture( s8 text_size )												   -> gl_id;
	returning get_character_info( wchar_t character, s8 text_size )						   -> Character_Info;
	returning get_kerning( wchar_t left_character, wchar_t right_character, s8 text_size ) -> v2;

private:
	compile_constant alphabet = UTF8_String{ 
		L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" // Latin chars
		L"ó!?()[]@#%*:;.,=<>" // '-' It's a dash, not a minus!! (U+2014)
		L"•∆ £—”åèØπÊÍ≥ÒÛúüø" // Polish characters
	}; 

	// Font file data has to be accessible for entire font lifetime
	// because we have to generate kerning info.
	Array<char> font_file_data;

	// Schrift library object.
	SFT sft;

	// We have to know which index corresponds to what text size.
	// Therefore, index here is index in every other array.
	// (we hold here Text_Size enum objects)
	Array<s8> text_sizes;

	// It's a long texture -- all characters are in single verse.
	Array<gl_id> textures;
	Array<Array<Character_Info>> character_infos;
};
}