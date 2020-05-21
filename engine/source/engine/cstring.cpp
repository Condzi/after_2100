#include <engine/cstring.hpp>

#include <cstring>

namespace con
{
CString::CString( char const* const runtime_str, s32 size_ ) :
	data( runtime_str ),
	size( size_ )
{}

CString& CString::operator=( CString const& other )
{
	std::memcpy( this, &other, sizeof( CString ) );

	return *this;
}
}