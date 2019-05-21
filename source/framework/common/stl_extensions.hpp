/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con
{
struct Find_Result final
{
	bool found{ false };
	s32 idx{ -1 };
};

template <typename TContainer, typename TValue>
[[nodiscard]] Find_Result find( TContainer const& container, TValue const& value );

template <typename TContainer, typename TLambda>
[[nodiscard]] Find_Result find_if( TContainer const& container, TLambda&& lambda );

template <typename TContainer, typename TLambda>
void remove_if( TContainer const& container, TLambda&& lambda );

bool string_begins_with( std::string const& string, std::string const& prefix );
}

#include "stl_extensions.inl"