/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con
{
template <typename T>
struct Find_Result
{
	bool found{ false };
	s32 idx{ -1 };
	T const *const value{ nullptr };
};

template <typename TContainer, typename TValue>
Find_Result<TValue> find( TContainer const& container, TValue const& value );

template <typename TValue, template<typename> class TContainer, typename TLambda>
Find_Result<TValue> find_if( TContainer<TValue> const& container, TLambda&& lambda );

}

#include "find.inl"