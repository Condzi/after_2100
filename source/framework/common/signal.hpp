/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con
{

// connect(...) returns function which has to be called when given funciton is invalidated.
// for example when node is being destroyed.
template <typename ...TArgs>
class Signal final 
{
	using Function = std::function<void( TArgs... )>;

public:
	using Disconnector = std::function<void()>;

	Signal();

	[[nodiscard]] auto connect( Function function ) -> Disconnector;
	void emit( TArgs ...args );

private:
	compile_constant RESERVED = 8;

	size_t                unique_id_counter{ 0 };
	std::vector<Function> functions;
};
}

#include "signal.inl"