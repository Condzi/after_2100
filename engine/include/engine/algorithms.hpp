#pragma once

#include <engine/typedefs.hpp>
#include <engine/array.hpp>

namespace con
{
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

template <typename TType, typename TComparator>
void insertion_sort( Array<TType>& arr, TComparator comparator );

//
//	Definitions
//

template <typename TType, typename TPredicate>
returning linear_find_if( Array<TType> const& arr, TPredicate predicate ) -> Find_Result
{
	constant size = arr.size();
	s32 idx = 0;

	while ( idx < size &&
			predicate( arr[idx] ) == false ) {
		++idx;
	}

	idx = ( idx == size ) ? -1 : idx;

	return { .idx = idx };
}

template <typename TType>
returning linear_find( Array<TType> const& arr, TType const& val ) -> Find_Result
{
	constant size = arr.size();
	s32 idx = 0;

	while ( idx < size &&
			arr[idx] != val ) {
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

template<typename TType, typename TComparator>
void insertion_sort( Array<TType>& arr, TComparator comparator )
{
	for ( s32 i = 1; i < arr.size(); ++i ) {
		for ( s32 j = i; 
			  j > 0 && comparator( arr[j-1], arr[j] ); 
			  --j ) {
			std::swap( arr[j], arr[j-1] );
		}
	}
}
}