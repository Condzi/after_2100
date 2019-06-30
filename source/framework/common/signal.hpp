/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "framework/scene/node.hpp"
#include "framework/common/stl_extensions.hpp"

namespace con
{
namespace priv
{
// Used by G_App to keep track of invalid signals and invalid subscribers in that signals.
class Signal_Base
{
public:
	Signal_Base();
	~Signal_Base();

	virtual void remove_invalid_subscribers() = 0;
};
}
template <typename ...TArgs>
class Signal final :
	public priv::Signal_Base
{
	using Function = std::function<void( TArgs... )>;

	struct Subscriber final
	{
		Node* bonded_node;
		Function function_to_call;

		Subscriber( Node& node, Function&& f );
	};

public:
	void bond( Node& node, Function&& function );
	void notify( TArgs ...args );

private:
	std::vector<Subscriber> subscribers;

	void remove_invalid_subscribers() override;
};
}

#include "signal.inl"