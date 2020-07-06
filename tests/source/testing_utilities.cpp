#include <testing_utilities.hpp>

#include <engine/typedefs.hpp>
#include <engine/context.hpp>
#include <engine/allocator.hpp>

using namespace con;

file_scope
{
	Default_Allocator   default_allocator;
	Temporary_Allocator temporary_allocator;
	C_Allocator			c_allocator;
	Stack_Allocator		stack_allocator;
}

Scoped_Test_Initializer::Scoped_Test_Initializer()
{
	Context.default_allocator   = &default_allocator;
	Context.temporary_allocator = &temporary_allocator;
	Context.c_allocator		    = &c_allocator;
	Context.stack_allocator		= &stack_allocator;

	default_allocator.initialize();
	temporary_allocator.initialize();
	stack_allocator.initialize();
}

Scoped_Test_Initializer::~Scoped_Test_Initializer()
{
	default_allocator.shutdown();
	stack_allocator.shutdown();

	Context.default_allocator   = nullptr;
	Context.temporary_allocator = nullptr;
	Context.c_allocator		    = nullptr;
	Context.stack_allocator		= nullptr;
}
