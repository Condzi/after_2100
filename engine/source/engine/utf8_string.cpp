#include <engine/utf8_string.hpp>

namespace con
{
UTF8_String::UTF8_String( wchar_t const* const runtime_str, s32 size_ ) :
	data( runtime_str ),
	length( size_ )
{}

UTF8_String& UTF8_String::operator=( UTF8_String const& other )
{
	memcpy( this, &other, sizeof( UTF8_String ) );

	return *this;
}
}