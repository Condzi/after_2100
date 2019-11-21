/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "assertions.hpp"
#include "debug_flags.hpp"
#include "stl_extensions.hpp"

namespace con::priv
{
void Debug_Flags::enable_all()
{
	for ( auto& flag : flags )
		if ( flag.name is_not "display_debug_console" )
			flag.status = true;
}

void Debug_Flags::disable_all()
{
	for ( auto& flag : flags )
		if ( flag.name is_not "display_debug_console" )
			flag.status = false;
}

auto Debug_Flags::get( std::string_view name ) -> bool&
{
	G_Profile_Function();

	constant hash = type_hash( name );

	constant[found, idx] = find_if( flags, [hash]( constant flag ) {
		return flag.hash == hash;
									} );

	report_warning_if( not found )
	{
		static bool fallback{ false };
		return fallback;
	}

	return flags[idx].status;
}

auto Debug_Flags::get_flags_names() -> std::vector<std::string>
{
	std::vector<std::string> vec;
	vec.resize( flags.size() );

	for ( size_t i = 0; i < vec.size(); i++ )
		vec[i] = flags[i].name;

	return vec;
}

void Debug_Flags::toggle( std::string_view name )
{
	bool& status = get( name );
	status = !status;
}

Debug_Flags& Debug_Flags::get_instance()
{
	static Debug_Flags* instance = new Debug_Flags;

	return *instance;
}
}