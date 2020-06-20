#pragma once

#include <engine/array.hpp>
#include <engine/hashed_cstring.hpp>

namespace con
{
class Config_File final
{
public:
	void parse_from_file( CString path );
	void parse_from_source( CString source );

	// Acts like a dtor but we have control over its lifetime.
	void free();

	returning get_value( Hashed_CString section, Hashed_CString name ) -> CString;

private:
	struct Config_Value_Info final
	{
		u32 section_hash;
		u32 name_hash;
		CString value;
	};

	// @Performance: We can probably have something like "here are the values from section of hash "0xdeadbeef": 
	// (range begin, range end). Using that we could have faster searching since we would know where to start and 
	// where to end search.	
	Array<Config_Value_Info> config_values;

	compile_constant section_mark = CString{ CON_CONFIG_SECTION_MARK };
	compile_constant comment_mark = CString{ CON_CONFIG_COMMENT_MARK };
};
}
