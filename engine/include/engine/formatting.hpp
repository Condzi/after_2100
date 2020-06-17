#pragma once

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

template <typename T>
returning cstring_to_T( CString str ) -> T;

//
//	Definitions
//

template <typename T>
returning T_to_cstring( T value ) -> CString
{
	if constexpr ( std::is_same_v<T, CString> ) {
		return value;
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
			con_assert( current_arg == args_count - 1 );
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

		con_assert( fs_it == final_string_size );

		return { final_string_buffer, final_string_size };
	}
}

template <typename T>
returning cstring_to_T( CString str ) -> T
{
	if constexpr ( std::is_same_v<T, CString> ) {
		return str;
	} else if constexpr ( std::is_integral_v<T> ) {
		return cstring_to_s32( str );
	} else if constexpr ( std::is_floating_point_v<T> ) {
		return cstring_to_f32( str );
	} else {
		static_assert( false, "Unsupported type (CString to T)." );
	}
}
}