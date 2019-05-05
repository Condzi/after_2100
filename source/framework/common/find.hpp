/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con
{
struct Find_Result
{
	bool found{ false };
	s32 idx{ -1 };
};

template <typename TContainer, typename TValue>
Find_Result find( TContainer const& container, TValue const& value );

template <typename TContainer, typename TLambda>
Find_Result find_if( TContainer const& container, TLambda&& lambda );

}

#include "find.inl"