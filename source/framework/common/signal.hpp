/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con
{

// bond(...) returns function which has to be called when given funciton is invalidated.
// for example when node is being destroyed.
template <typename ...TArgs>
class Signal final 
{
	using Function = std::function<void( TArgs... )>;

public:
	using Disconnector = std::function<void()>;

	[[nodiscard]] auto connect( Function function ) -> Disconnector;

	void emit( TArgs ...args );

private:
	size_t                               unique_id_counter{ 0 };
	std::unordered_map<size_t, Function> functions;
};
}

#include "signal.inl"