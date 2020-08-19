#include <engine/config_file.hpp>
#include <engine/allocator.hpp>
#include <engine/logger.hpp>
#include <engine/utility.hpp>

#include <cstring>

namespace con
{
returning Config_File::parse_from_file( CString path ) -> bool
{
	con_assert( config_values.size() <= 0 ); // only one config file per parse for now.

	// We're allocating stuff here using temporary allocator.
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	constant[file_content, success] = load_entire_file_binary( path );

	if ( !success ) {
		return false;
	}

	//
	// Get the config entries.
	//

	config_values.initialize( CON_MAX_CONFIG_ENTRIES, Context.default_allocator );

	u32 current_section_hash = 0;
	// Count lines which aren't empty, aren't a comment and aren't a section beginning.
	// We use this value to determine how much memory we need for `config_values`.
	s32 current_config_value = 0;

	for ( s32 idx = 0; idx < file_content.size(); ) {
		idx = ate_whitespace( file_content, idx );
		if ( idx == file_content.size() ) {
			break;
		}
		constant endline_idx = ate_chars_until( file_content, idx, '\n' );
		CString const temp{ file_content.data() + idx, endline_idx - idx };

		if ( temp.begins_with( comment_mark ) ) {
			idx = endline_idx;
			continue;
		} else if ( temp.begins_with( section_mark ) ) {
			constant section_name_begin = idx + section_mark.length;
			constant section_name_end = ate_chars_until_whitespace( file_content, section_name_begin );
			current_section_hash = hash_cstring( { file_content.data() + section_name_begin, section_name_end - section_name_begin } );
			idx = endline_idx;
			continue;
		}

		constant name_idx_begin = idx;
		constant name_idx_end = ate_chars_until_whitespace( file_content, idx );
		CString const name{ file_content.data() + name_idx_begin, name_idx_end - name_idx_begin };
		constant name_hash = hash_cstring( name );
		constant value_idx = ate_whitespace( file_content, name_idx_end );
		// No value assigned.
		if ( value_idx >= endline_idx ) {
			// We don't need this warning since .scene_resources format allows, even requires, that.
			// It's still forbidden in local.variables file, tho.
			// con_log_indented( 1, R"(Warning: in file "%" field "%" has no value assigned!)", path, name );

			config_values[current_config_value] ={
				.section_hash = current_section_hash,
				.name_hash = name_hash,
				.value = {}
			};

			idx = endline_idx;
			++current_config_value; // We're wasting space here but no value is a bug either way so it's not bad. 
			continue;
		}
		constant value_str_size = ( ate_whitespace_reversed( file_content, endline_idx - 1 ) + 1 ) - value_idx;

		char* value_buffer = reinterpret_cast<char*>( Context.default_allocator->allocate( value_str_size ) );
		memcpy( value_buffer, file_content.data() + value_idx, value_str_size );

		CString const value{ value_buffer, value_str_size };

		config_values[current_config_value] ={
			.section_hash = current_section_hash,
			.name_hash = name_hash,
			.value = value
		};
		idx = endline_idx;

		++current_config_value;
		if ( current_config_value >= config_values.size() ) {
			con_push_indent();
			con_log( R"(Error: our limit for config values, %, has been exceeded! File: "%".)", CON_MAX_CONFIG_ENTRIES, path );
			con_pop_indent();
			return false;
		}
	}

	config_values.shrink( current_config_value );

	return true;
}

returning Config_File::parse_from_source( CString source ) -> bool
{
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	// @Performance: We can avoid this by having methods that accept CString instead of Array<char>.
	// This will do it for now tho.
	Array<char> source_content;
	source_content.initialize( source.length, temporary_allocator );
	memcpy( source_content.data(), source.data, source.length );

	config_values.initialize( CON_MAX_CONFIG_ENTRIES, Context.default_allocator );

	u32 current_section_hash = 0;
	// Count lines which aren't empty, aren't a comment and aren't a section beginning.
	// We use this value to determine how much memory we need for `config_values`.
	s32 current_config_value = 0;

	for ( s32 idx = 0; idx < source_content.size(); ) {
		idx = ate_whitespace( source_content, idx );
		if ( idx == source_content.size() ) {
			break;
		}
		constant endline_idx = ate_chars_until( source_content, idx, '\n' );
		CString const temp{ source_content.data() + idx, endline_idx - idx };

		if ( temp.begins_with( comment_mark ) ) {
			idx = endline_idx;
			continue;
		} else if ( temp.begins_with( section_mark ) ) {
			constant section_name_begin = idx + section_mark.length;
			constant section_name_end = ate_chars_until_whitespace( source_content, section_name_begin );
			current_section_hash = hash_cstring( { source_content.data() + section_name_begin, section_name_end - section_name_begin } );
			idx = endline_idx;
			continue;
		}

		constant name_idx_begin = idx;
		constant name_idx_end = ate_chars_until_whitespace( source_content, idx );
		CString const name{ source_content.data() + name_idx_begin, name_idx_end - name_idx_begin };
		constant name_hash = hash_cstring( name );
		constant value_idx = ate_whitespace( source_content, name_idx_end );
		if ( value_idx >= endline_idx ) {
			con_log( R"(Warning: in DEFAULT CONFIG field "%" has no value assigned!)", name );
			idx = endline_idx;
			++current_config_value; // We're wasting space here but no value is a bug either way so it's not bad. 
			continue;
		}
		constant value_str_size = ( ate_whitespace_reversed( source_content, endline_idx - 1 ) + 1 ) - value_idx;

		char* value_buffer = reinterpret_cast<char*>( Context.default_allocator->allocate( value_str_size ) );
		memcpy( value_buffer, source_content.data() + value_idx, value_str_size );

		CString const value{ value_buffer, value_str_size };

		config_values[current_config_value] ={
			.section_hash = current_section_hash,
			.name_hash = name_hash,
			.value = value
		};
		idx = endline_idx;

		++current_config_value;
		if ( current_config_value >= config_values.size() ) {
			con_log( "(Error: our limit for config values, %, has been exceeded! (in DEFAULT CONFIG)", CON_MAX_CONFIG_ENTRIES );
			return false;
		}
	}

	config_values.shrink( current_config_value );

	return true;
}

void Config_File::free()
{
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( config_values[i].value.data ) ), config_values[i].value.length );
	}

	config_values.shutdown();
}

returning Config_File::get_value( Hashed_CString section, Hashed_CString name ) const -> CString
{
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		auto& info = config_values[i];
		if ( info.section_hash == section.hash &&
			 info.name_hash == name.hash ) {
			return info.value;
		}
	}

	return {};
}

returning Config_File::get_section( Hashed_CString section ) const -> Array<Hash_Value_Pair>
{
	// We're allocating on TS, but only once -- when we initialize the to_return
	// array. Because of that, it's legal to call set_mark to free the storage memory.
	// It ain't much, but it's always something!

	Array<Hash_Value_Pair> to_return;
	// In worst case (which won't happen), we return (copy) all stored values.
	to_return.initialize( config_values.size(), Context.temporary_allocator );

	// We use this value to see how much memory we can free.
	s32 current_entry = 0;
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		constant& cfg_val = config_values[i];
		if ( cfg_val.section_hash == section.hash ) {
			auto& entry = to_return[current_entry];

			entry.hash = cfg_val.name_hash;
			entry.value = cfg_val.value;
			++current_entry;
		}
	}

	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

	// We didin't find any matching entry, so we return the requested memory back to
	// the TA.
	if ( current_entry <= 0 ) {
		ta.set_mark( ta.get_mark() - to_return.size() );
		return {};
	}

	// Shrink the array and "return" the unused memory by moving the mark.
	if ( to_return.size() > current_entry ) {
		to_return.shrink( current_entry );
		ta.set_mark( ta.get_mark() - ( config_values.size() - current_entry ) );
	}

	return to_return;
}
returning Config_File::get_all_section_hashes() const -> Array<u32>
{
	// We can have at most config_values.size sections (1 section/1 value )
	Array<u32> to_return;
	to_return.initialize( config_values.size(), Context.temporary_allocator );

	// At the end is equal to final size of to_return array.
	s32 current_entry = 0;
	// @Robustness: it's unlikely to even happen but we don't check for duplicates!
	u32 current_hash_value = 0;
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		constant& hash = config_values[i].section_hash;

		if ( current_hash_value != hash ) {
			current_hash_value = hash;
			to_return[current_entry] = hash;
			++current_entry;
		}
	}

	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

	// No entries or what? Why would there even be no entries?
	if ( current_entry <= 0 ) {
		ta.set_mark( ta.get_mark() - to_return.size() );
		return {};
	}

	// Shrink the array and "return" the unused memory by moving the mark.
	to_return.shrink( current_entry );
	ta.set_mark( ta.get_mark() - ( config_values.size() - current_entry ) );

	return to_return;
}
}