/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

namespace con
{
using Signal_Disconnector = std::function<void()>;

// connect(...) returns function which has to be called when given funciton is invalidated.
// for example when node is being destroyed.
template <typename ...TArgs>
class Signal final
{
	using Function = std::function<void( TArgs... )>;

public:
	Signal();

	[[nodiscard]] auto connect( Function function ) -> Signal_Disconnector;
	void emit( TArgs ...args );

private:
	compile_constant RESERVED = 8;

	std::vector<Function> functions;
};
}

#include "signal.inl"