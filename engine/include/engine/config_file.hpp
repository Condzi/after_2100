#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>

namespace con
{
class Config_File final
{
public:
	void parse( CString path );

	~Config_File();

	returning get_value( CString section, CString name ) -> CString;

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
