#pragma once

#include <engine/typedefs.hpp>
#include <engine/array.hpp>

namespace con
{
// Structure returned by every find function.
struct Find_Result final
{
	s32 const idx = -1;

	returning found() const -> bool
	{
		return idx >= 0;
	}
};

//
//	Declarations
//
template <typename TType, typename TPredicate>
returning linear_find_if( Array<TType> const& arr, TPredicate predicate ) -> Find_Result;

template <typename TType>
returning linear_find( Array<TType> const& arr, TType const& val ) -> Find_Result;

// TComparator should just return if for given value a: a < value.
template <typename TType, typename TComparator, typename TPredicate>
returning binary_find_if( Array<TType> const& arr, TComparator comparator, TPredicate predicate ) -> Find_Result;

//
//	Definitions
//

template <typename TType, typename TPredicate>
returning linear_find_if( Array<TType> const& arr, TPredicate predicate ) -> Find_Result
{
	constant size = arr.size();
	s32 idx = 0;

	while ( predicate( arr[idx] ) == false &&
			idx < size ) {
		++idx;
	}

	idx = ( idx == size ) ? -1 : idx;

	return { .idx = idx };
}

template <typename TType>
returning linear_find( Array<TType> const & arr, TType const & val ) -> Find_Result
{
	constant size = arr.size();
	s32 idx = 0;

	while ( arr[idx] != val &&
			idx < size ) {
		++idx;
	}

	idx = ( idx == size ) ? -1 : idx;

	return { .idx = idx };
}

template <typename TType, typename TComparator, typename TPredicate>
returning binary_find_if( Array<TType> const& arr, TComparator comparator, TPredicate predicate ) -> Find_Result
{
	s32 left = 0;
	s32 right = arr.size();
	s32 middle = 0;

	while ( left <= right ) {
		middle = left + ( right - left ) / 2;

		if ( predicate( arr[middle] ) ) {
			return { .idx = middle };
		}

		if ( comparator( arr[middle] ) ) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}

	return { .idx = -1 };
}
}