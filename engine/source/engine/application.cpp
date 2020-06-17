#include <engine/application.hpp>

namespace con
{
returning Application::initialize() -> Initialization_Error_Code
{
	Context.default_allocator = &default_allocator;
	Context.temporary_allocator = &temporary_allocator;
	Context.entity_manager = &entity_manager;
	Context.logger = &main_logger;

	default_allocator.initialize();
	temporary_allocator.initialize();
	// @ToDo: we have to initialize it's arrays.
	// entity_manager.initialze() ?
	main_logger.initialize();

	// @ToDo: Load config here and fallback to default values if necessary
	// @ToDo: Splash screen stuff?? in separate thread? use it 
	// @ToDo: Initialize window here.
	// @ToDo: Load resources here.

	con_log( "Application initialized!" );
	return Initialization_Error_Code::Success;
}
void Application::run()
{
	con_log( "Application runs..." );

	// @ToDo: while(Context.flags.exit_flags.all_unset() == true ) ?
	return;
}

void Application::shutdown()
{
	con_log( "Application shutdown..." );
	// @ToDo: entity_manager.shutdown();
	// @ToDo: window.shutdown()...
	// @ToDo: Call last -- we may want to log some errors during shutdown. Also, 
	// don't forget to flush data here.
	main_logger.shutdown();
	default_allocator.shutdown();
}
}