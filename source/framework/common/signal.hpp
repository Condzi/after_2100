/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "framework/scene/node.hpp"

namespace con
{
namespace priv
{
// Used by G_App to keep track of invalid signals and invalid subscribers in that signals.
class Signal_Base
{
public:
	Signal_Base( Signal_Base const& ) = delete;
	Signal_Base& operator=( Signal_Base const& ) = delete;

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
		Node const& bonded_node;
		Function function_to_call;
	};

public:
	void bond( Node const& node, Function&& function )
	{
		subscribers.emplace_back( node, function );
	}

	void notify( TArgs&& ...args )
	{
		for ( auto& sub : subscribers )
			sub.function_to_call( std::forward<TArgs>( args )... );
	}


private:
	std::vector<Subscriber> subscribers;

	void remove_invalid_subscribers() override
	{
		remove_if( subscribers, []( constant& subscriber ) {
			return subscriber.bonded_node.is_queued_for_delete();
		} );
	}
};
}