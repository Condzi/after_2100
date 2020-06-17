#pragma once

#include <engine/typedefs.hpp>

// @Robustness: Do we want to include this stuff here? Maybe 
// move it to the cpp file to avoid cross include?
#include <engine/allocator.hpp>
#include <engine/entity_manager.hpp>
#include <engine/logger.hpp>

namespace con
{
class Application final
{
public:
	// @ToDo: this enum to string conversion, we just use it to print error
	// messages.
	enum class Initialization_Error_Code : s8
	{
		Success,
		Window_Creation_Failure
		// ... Other stuff ...
	};

	returning initialize() -> Initialization_Error_Code;
	// @Idea: Return codes? Different for exit because failure / crash?
	// Or maybe just check the Context's flags (flags.exit_flags[error_blah])?
	void run();
	void shutdown();

private:
	Default_Allocator default_allocator;
	Temporary_Allocator temporary_allocator;
	Entity_Manager entity_manager;
	Logger main_logger;
	// @ToDo: game timer here

	// @Idea: Global struct that contains values like that? Or just hold it in the 
	// config file? Or maybe just gather it once in the run() function from the global config file?
	f32 ups = 1.0f / 60; // = default value; @ToDo ! ! ! !

	// @ToDo: Add flushing of logger at the beginning of every frame!!!
	// void flush_logger();
};
}