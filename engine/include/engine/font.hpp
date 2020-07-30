#pragma once

#include <engine/array.hpp>
#include <engine/text_size.hpp>
#include <engine/hashed_cstring.hpp>

#include <schrift/schrift.h>

namespace con
{
// Basically just a container for character texture atlas.
class Font final
{
public:
	// Every font should know what sizes does it need.
	// For example developer console font (Cascadia Code right now)
	// has only one size, but other fonts may have to have more than 
	// one size.
	//
	// text_sizes should be an array of Text_Size::X enum.
	void initialize( Hashed_CString name, Array<s8> const& text_sizes );
	void initialize( Hashed_CString name, s8 single_text_size );
	void shutdown();

	// text_size = -1 means that we have only one text size.
	returning get_kerning( wchar_t left_character, wchar_t right_character, s8 text_size = -1 ) -> v2;

private:
	constexpr static wchar_t alphabet[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" // Latin chars
		L"ó!?()[]@#%*:;.,=<>" // '-' It's a dash, not a minus!! (U+2014)
		L"•∆ £—”åèØπÊÍ≥ÒÛúüø"; // Polish characters
	compile_constant alphabet_length = static_cast<s32>( sizeof( alphabet ) / sizeof( wchar_t ) );


	// Schrift library object.
	SFT sft;

	// One texture per one text size. Sometimes we just need one text/font size.
	bool has_single_text_size = true;

	// We have to know which index corresponds to what text size.
	// Therefore, index here is index in every other array.
	// If the Font has only one text size, this field is irrelevant and not
	// initialized. 
	Array<s8> text_sizes;

	struct Character_Info final
	{
		s16 width  = -1;
		s16 height = -1;
		s16 offset = -1;
	};

	// It's a long texture -- all characters are in single verse.
	union
	{
		gl_id single_texture = 0;
		Array<gl_id> textures;
	};

	union
	{
		// Index here corresponds to index in the alphabet array.
		// (we have to lookup position in the alphabet first, then 
		// this index is valid in this array)
		Array<Character_Info> single_character_info;
		Array<Array<Character_Info>> character_infos;
	};
};
}