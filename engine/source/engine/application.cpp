#include <engine/application.hpp>

#include <engine/default_config_values.hpp>
#include <engine/scene_resources_file_parser.hpp>

#include <filesystem>

namespace con
{
returning Application::initialize() -> bool
{
	Context.default_allocator = &default_allocator;
	Context.temporary_allocator = &temporary_allocator;
	Context.c_allocator = &c_allocator;
	Context.stack_allocator = &stack_allocator;
	Context.entity_manager = &entity_manager;
	Context.logger = &main_logger;
	Context.config_file = &config_file;
	Context.input = &input;
	Context.prepared_resources = &prepared_resources;

	default_allocator.initialize();
	temporary_allocator.initialize();
	stack_allocator.initialize();
	// @ToDo: we have to initialize it's arrays.
	// entity_manager.initialze() ?
	main_logger.initialize();

	con_log( "Initializing main logger..." );
	set_up_log_folder();
	main_logger_file = std::fopen( CON_DEFAULT_LOG_FILE, "wb" );
	if ( main_logger_file == nullptr ) {
		con_log_indented( 1, "Couldn't open log file \"%\"!", CString{ CON_DEFAULT_LOG_FILE } );
	} else {
		con_log_indented( 1, "Logger file successfully opened." );
	}

	con_log( "Checking necessary paths..." );
	if ( !check_necessary_paths() ) {
		return false;
	}
	con_log( "Paths are correct." );
	con_log( "Loading config file..." );
	if ( !std::filesystem::exists( CON_CONFIG_FILE ) ) {
		con_log_indented( 1, "Config file couldn't be found at \"%\", loading the default one." );
		config_file.parse_from_source( DEFAULT_CONFIG_CSTRING );
	} else {
		// @Robustness: I'm fired. It's a stupid-ass idea to not return a bool
		// here. Just load the default config if this failes.
		config_file.parse_from_file( CON_CONFIG_FILE );
	}
	flush_logger();

	con_log_indented( 1, "(FIXME) Probably done loading config file. IDK because I'm fired" );

	// @ToDo: Splash screen stuff?? in separate thread? use it 

	con_log( "Initializing window..." );
	window.initialize();
	con_log( "Window initialized." );
	flush_logger();

	con_log( "Initializing resource loader..." );
	resource_loader.initialize();
	con_log( "Resource loader initialized." );

	flush_logger();

	con_log( "Initializing input..." );
	input.initialize( window );
	con_log( "Input initialized." );
	flush_logger();


	con_log( "Initialization completed." );
	flush_logger();
	return true;
}

void Application::run()
{
	con_log( "Application runs..." );

	input.add_binding( "exit_button"_hcs, Key::Keyboard_ESCAPE );

	Time_Period frame_start = Time::now();
	Time_Period frame_end;
	f32 frame_dt = 0;
	f32 accumulated_dt = 0;
	constant ups = 1.0f / cstring_to_s32( config_file.get_value( "gameplay"_hcs, "ups"_hcs ) );


	// @ToDo: Stop also when there is a GL or GLFW error.
	while ( Context.engine_flags.exit == false ) {
		if ( window.should_close() ) {
			// @ToDo: Add flags to distinguish what type of exit we have -- is it
			// because of error, closed window (alt+F4) or because player clicks 'Exit' 
			// button in main menu?
			Context.engine_flags.exit = true;
		}

		if ( input.is_key_pressed( "exit_button"_hcs ) ) {
			Context.engine_flags.exit = true;
		}

		input.poll_events();

		accumulated_dt += frame_dt;
		while ( accumulated_dt >= ups ) {
			// Physic update here...
			// world.update_physic(ups)...
			accumulated_dt -= ups;
		}

		// normal update here...
		// world.update(frame_dt);

		window.clear();
		window.display();

		flush_logger();
		temporary_allocator.set_mark( 0 );

		frame_end = Time::now();
		frame_dt = Time::to_seconds( Time::difference( frame_start, frame_end ) );
		frame_start = frame_end;
	}
}

void Application::shutdown()
{
	flush_logger();
	con_log( "Application shutdown..." );
	config_file.free();
	window.shutdown();
	resource_loader.shutdown();
	input.shutdown();
	// @ToDo: entity_manager.shutdown();
	con_log( "Highest TA mark: % / % (bytes).", temporary_allocator.get_highest_mark(), CON_TEMPORARY_STORAGE_RESERVED_MEMORY );
	flush_logger(); // flushing last messages here...
	main_logger.shutdown();
	std::fclose( main_logger_file );
	default_allocator.shutdown();
	stack_allocator.shutdown();
}

void Application::flush_logger()
{
	// @ToDo: Check if we're in release mode. If yes, don't try to log to console.
	constant data_to_log = main_logger.get_buffer();
	if ( data_to_log.size == 0 ) {
		return;
	}

	std::fputs( data_to_log.data, stdout );
	if ( main_logger_file != nullptr ) {
		std::fputs( data_to_log.data, main_logger_file );
	}
	main_logger.reset_buffer();
}

returning Application::set_up_log_folder() -> bool
{
	namespace fs = std::filesystem;

	if ( !fs::exists( CON_LOGS_FOLDER ) ) {
		con_log_indented( 1, "Logs folder is missing. Creating \"%\".", CString{ CON_LOGS_FOLDER } );
		if ( !fs::create_directory( CON_LOGS_FOLDER ) ) {
			con_log_indented( 1, "Couldn't create!!!" );
			return false;
		}
	}
	return true;
}

returning Application::check_necessary_paths() const -> bool
{
	namespace fs = std::filesystem;
	bool success = true;
	constant path_exists = [&success]( CString path ) {
		if ( fs::exists( { path.data, path.data + path.size } ) == false ) {
			con_log_indented( 1, "Error! Missing path \"%\".", path );
			success = false;
		}
	};


	path_exists( CON_DATA_FOLDER );
	path_exists( CON_ASSETS_FOLDER );
	path_exists( CON_TEXTURES_FOLDER );

	return success;
}
}