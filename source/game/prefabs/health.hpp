/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/common/signal.hpp"
#include "framework/scene/node.hpp"

using namespace con;

class Health :
	public Node
{
	CLASS_DEF( Health );

public:
	// Has Node* parameter, which is optional and can be nullptr.
	// s32 is how many HP is given/received. It's value fiven by other
	// entity, not calculated. So hp = 10, dmg = 100 -> the value of s32 will be 100, not 10.
	Signal<s32, Node*> s_heal;
	Signal<s32, Node*> s_damage;
	Signal<>           s_refill;
	Signal<>           s_dead;

	Health();

	bool is_immoral{ false };

	auto get_max() const     -> s32;
	auto get_current() const -> s32;

	void set_max( s32 max_value );

	bool is_dead() const;

	// If entity is dead, i.e. current hp = 0, damage() doesnt work.
	void heal( s32 value, Node* healer );
	void damage( s32 value, Node* dealer = nullptr );
	void refill();
	void kill( Node* killer = nullptr );

private:
	s32 max_hp{ 1 };
	s32 current_hp{ 1 };
};