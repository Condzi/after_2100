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
	[[nodiscard]] auto bond( Function&& function ) -> std::function<void()>;
	void emit( TArgs ...args );

private:
	std::vector<Function> functions;
};
}

#include "signal.inl"