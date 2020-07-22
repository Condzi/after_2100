#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>
#include <engine/allocator.hpp>
#include <engine/context.hpp>

#include <type_traits>
#include <cstring>

namespace con
{
//
//	Value -> CString
//
returning bool_to_cstring( bool value ) -> CString;
returning s32_to_cstring( s32 value ) -> CString;
returning f32_to_cstring( f32 value ) -> CString;

template <typename T>
returning T_to_cstring( T value ) -> CString;

// @ToDo: benchmark against C
template <typename ...TArgs>
returning sprint( CString fmt, TArgs ...args ) -> CString;

//
//	CString -> Value
//
returning cstring_to_s32( CString str ) -> s32;
returning cstring_to_f32( CString str ) -> f32;
returning cstring_to_bool( CString str ) -> bool;

template <typename T>
returning cstring_to_T( CString str ) -> T;

// C style of parsing, still useful (used in sscan implementation for folding expressions)
template <typename T>
void cstring_to_T( CString str, T& t );

// Returns true on successfull parsing.
template <typename ...TArgs>
returning sscan( CString format, CString str, TArgs& ...args ) -> bool;

//
//	Definitions
//

template <typename T>
returning T_to_cstring( T value ) -> CString
{
	if        constexpr ( std::is_same_v<T, CString> ) {
		return value;
	} else if constexpr ( std::is_same_v<T, bool> ) {
		return bool_to_cstring( value );
	} else if constexpr ( std::is_integral_v<T> ) {
		return s32_to_cstring( static_cast<s32>( value ) );
	} else if constexpr ( std::is_floating_point_v<T> ) {
		return f32_to_cstring( static_cast<f32>( value ) );
	} else {
		static_assert( false, "Unsupported type (T to CString). Maybe you forget _cs?" );
	}
}

template <typename ...TArgs>
returning sprint( CString fmt, TArgs ...args ) -> CString
{
	compile_constant args_count = sizeof...( TArgs );

	// @Robustness: should we allocate or should we not? hmmm
	if constexpr ( args_count == 0 ) {
		return fmt;
	} else {
		Temporary_Allocator& temporary_allocator = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

		static CString str_args[args_count];
		{
			s32 current_arg{ -1 };
			( void( str_args[++current_arg] = T_to_cstring( args ) ), ... );
			assert( current_arg == args_count - 1 );
		}

		constant final_string_size = [&] {
			s32 size = fmt.size - args_count; // - args_count because we don't want to count '%'
			for ( constant str : str_args ) {
				size += str.size;
			}
			return size;
		}( );

		char* final_string_buffer = temporary_allocator.allocate<char>( final_string_size );

		s32 fmt_it, fs_it, arg_it;
		fmt_it = fs_it = arg_it = 0;
		for ( ; fmt_it < fmt.size; ++fmt_it ) {
			if ( fmt.data[fmt_it] == '%' && arg_it < args_count ) {
				std::memcpy( final_string_buffer + fs_it, str_args[arg_it].data, str_args[arg_it].size );
				fs_it += str_args[arg_it].size;
				++arg_it;
			} else {
				final_string_buffer[fs_it] = fmt.data[fmt_it];
				++fs_it;
			}
		}

		assert( fs_it == final_string_size );

		return { final_string_buffer, final_string_size };
	}
}

template <typename T>
returning cstring_to_T( CString str ) -> T
{
	if constexpr ( std::is_same_v<T, CString> ) {
		return str;
	} else if constexpr ( std::is_integral_v<T> ) {
		if constexpr ( std::is_same_v<T, bool> ) {
			return cstring_to_bool( str );
		} else {
			return cstring_to_s32( str );
		}
	} else if constexpr ( std::is_floating_point_v<T> ) {
		return cstring_to_f32( str );
	} else {
		static_assert( false, "Unsupported type (CString to T)." );
	}
}

template <typename T>
void cstring_to_T( CString str, T& t )
{
	t = cstring_to_T<T>( str );
}

template <typename ...TArgs>
returning sscan( CString format, CString str, TArgs& ...args ) -> bool
{
	compile_constant args_count = sizeof...( TArgs );

	static_assert( args_count > 0, "No arguments for sscan" );

	// @ToDo: reset mark?
	Temporary_Allocator& temporary_allocator = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant ta_mark = temporary_allocator.get_mark();
	defer{ temporary_allocator.set_mark( ta_mark ); };

	Array<CString> strings_to_parse;
	strings_to_parse.initialize( args_count, &temporary_allocator );

	// i = idx in format; j = idx in str
	s32 current_arg = 0;
	for ( s32 i = 0, j = 0; i < format.size && j < str.size && current_arg < args_count; ) {
		// Skip matching characters
		while ( format.data[i] == str.data[j] ) {
			++i;
			++j;
		}

		// Fail if character that differenciate is not a %.
		assert( format.data[i] == '%' );
		if ( format.data[i] != '%' ) {
			return false;
		}
		// Don't forget to increment index in the format string!
		++i;

		constant start = j;
		// Skip characters that make the number until you find the next format character or 
		// end of the string.
		while ( str.data[++j] != format.data[i] && j < str.size );
		constant size = j - start;
		strings_to_parse[current_arg] = CString{ str.data +start, size };
		++current_arg;
	}

	// We haven't found all of the arguments.
	assert( current_arg == args_count );

	current_arg = -1; // -1 because folding expressions.

	( void( cstring_to_T( strings_to_parse[++current_arg], args ) ), ... );

	return true;
}
}