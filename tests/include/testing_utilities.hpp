#pragma once

#include <catch/catch.hpp>

// Initializes Context stuff and allocator stuff which was
// kinda redundant because we were doing it for every test.
struct Scoped_Test_Initializer final
{
	Scoped_Test_Initializer();
	~Scoped_Test_Initializer();
};