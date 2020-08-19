#include <engine/application.hpp>

#include <engine/default_config_values.hpp>
#include <engine/scene_resources_file_parser.hpp>

#include <filesystem>

namespace con
{
returning Application::initialize() -> bool
{
	//
	// Assiging the Context pointers.
	//

	Context.default_allocator   = &default_allocator;
	Context.temporary_allocator = &temporary_allocator;
	Context.c_allocator         = &c_allocator;
	Context.stack_allocator     = &stack_allocator;
	Context.entity_manager      = &entity_manager;
	Context.logger              = &main_logger;
	Context.config_file         = &config_file;
	Context.input               = &input;
	Context.prepared_resources  = &prepared_resources;
	Context.renderer            = &renderer;
	Context.window              = &window;
	Context.dev_console         = &dev_console;

	//
	// Initialize basic systems.
	//

	default_allocator.initialize();
	temporary_allocator.initialize();
	stack_allocator.initialize();
	main_logger.initialize();
	dev_console.initialize_buffers();

	// @Robustness: well, it turns out we sometimes want to initialize memory.
	prepared_resources ={};


	con_log( "Version " CON_VERSION_STRING " compiled on " __DATE__ " at " __TIME__ " CET." );
	con_log( "Initialization started..." );
	con_push_indent();

	//
	// Initialize the logger and open it's log file.
	//

	con_log( "Initializing main logger..." );
	con_push_indent();

	if ( !set_up_log_folder() ) {
		con_log( R"(Fatal error: couldn't initialize log folder at "%".)", CString{ CON_DEFAULT_LOG_FILE } );
		return false;
	}
	
	con_pop_indent();
	con_log( "Main logger initialized." );

	//
	// Check necessary paths (data/...) etc.
	//

	con_log( "Checking necessary paths..." );
	con_push_indent();

	if ( !check_necessary_paths() ) {
		return false;
	}

	con_pop_indent();
	con_log( "Paths are correct." );

	//
	// Load config file, local.variables.
	//

	con_log( "Loading config file..." );
	con_push_indent();

	std::error_code fs_error_code;
	constant config_file_exists = std::filesystem::exists( CON_CONFIG_FILE, fs_error_code );

	if ( fs_error_code || !config_file_exists ) {
		if ( fs_error_code ) {
			con_log( R"(Warning: couldn't check if config file exists at "%", loading config from memory.)", CString{ CON_CONFIG_FILE } );
		} else {
			con_log( R"(Warning: config file couldn't be found at "%", loading config from memory.)", CString{ CON_CONFIG_FILE } );
		}

		con_push_indent();
		defer{ con_pop_indent(); };
		if ( config_file.parse_from_source( DEFAULT_CONFIG_CSTRING ) ) {
			con_log( "Default config has been loaded correctly." );
		} else {
			con_log( "Fatal error: can't load even the default config! WTF happened??" );
			return false;
		}
	} else {
		constant parse_success = config_file.parse_from_file( CON_CONFIG_FILE );
		con_pop_indent();

		if ( parse_success ) {
			con_log( "Config file loaded and parsed." );
		} else {
			con_log( "Error: loading of the config file has failed." );
		}
	}


	// @ToDo: Splash screen stuff?? in separate thread?

	//
	// Window init.
	//

	con_log( "Initializing window..." );
	con_push_indent();

	window.initialize();

	con_pop_indent();
	con_log( "Window initialized." );

	//
	// We have to have window initialized to know the text size.
	//

	update_text_sizes();

	//
	// Resource Loader init.
	//

	con_log( "Initializing resource loader..." );
	con_push_indent();

	resource_loader.initialize();

	con_pop_indent();
	con_log( "Resource loader initialized." );

	//
	// Dev_Console init.
	//

	bool do_dev_console = false;
	constant dev_console_str = config_file.get_value( "debug"_hcs, "dev_console"_hcs );
	sscan( "%", dev_console_str, do_dev_console );

	if ( do_dev_console ){
		con_log( "Initializing developer console... ");
		con_push_indent();

		dev_console.initialize_graphics();

		con_pop_indent();
		con_log( "Developer console initialized." );
	}

	//
	// Input init.
	//

	con_log( "Initializing input..." );
	con_push_indent();

	input.initialize( window );
	
	con_pop_indent();
	con_log( "Input initialized." );

	//
	// Entity Manager init.
	//

	con_log( "Initializing entity manager..." );
	con_push_indent();

	entity_manager.initialize();

	con_pop_indent();
	con_log( "Entity manager initialized." );

	//
	// Renderer init.
	//

	con_log( "Initializing renderer..." );
	con_push_indent();

	renderer.initialize();
	renderer.set_window_size( window.width(), window.height() );

	con_pop_indent();
	con_log( "Renderer initialized." );


	//
	// Debug / Test stuff.
	//

	con_log( "Debug spawn player..." );
	con_push_indent();

	resource_loader.prepare_resources_for_scene( "sandbox" );
	entity_manager.spawn_entity<Player>();

	con_pop_indent();
	con_log( "Player spawned." );

	//
	// End of initializations.
	//

	con_pop_indent();
	con_log( "Initialization completed." );
	con_check( false );

	return true;
}

void Application::run()
{
	con_log( "Application runs..." );

	// @Robustness: move the add_binding calls to other place?
	input.add_binding( "exit_button"_hcs, Key::Keyboard_ESCAPE );
	input.add_binding( "dev_console"_hcs, Key::Keyboard_F1 );

	Time_Period frame_start = Time::now();
	Time_Period frame_end;
	f32 frame_dt = 0;
	f32 accumulated_dt = 0;
	ups = 1.0f / cstring_to_s32( config_file.get_value( "gameplay"_hcs, "ups"_hcs ) );

	while ( !( Context.exit_flags.requested_by_app ||
			Context.exit_flags.requested_by_user ) ) {

		if ( window.should_close() ) {
			Context.exit_flags.requested_by_user = true;
		}

		if ( input.is_key_pressed( "exit_button"_hcs ) ) {
			Context.exit_flags.requested_by_user = true;
		}

		input.poll_events();

		accumulated_dt += frame_dt;
		// We cut off too much accumulated_dt to avoid
		// huge jumps when laggy.
		// @Idea: move 4* to macro_config?
		/*
		if ( accumulated_dt > 4 * ups ) {
			accumulated_dt = 4 * ups;
		}
		*/

		while ( accumulated_dt >= ups ) {
			entity_manager.physic_update( ups );
			accumulated_dt -= ups;
		}


		entity_manager.frame_update( frame_dt );
		dev_console.update( frame_dt );

		window.clear();
		renderer.render();
		window.display();

		temporary_allocator.set_mark( 0 );

		frame_end = Time::now();
		frame_dt = Time::to_seconds( Time::difference( frame_start, frame_end ) );
		frame_start = frame_end;
	}
}

void Application::shutdown()
{
	con_log( "Application shutdown..." );

	con_push_indent();

	con_log( "Exit flags: " );
	con_log( "  requested_by_app  = %", Context.exit_flags.requested_by_app );
	con_log( "  requested_by_user = %", Context.exit_flags.requested_by_user );

	entity_manager.shutdown();
	renderer.shutdown();
	config_file.free();
	window.shutdown();
	resource_loader.shutdown();
	input.shutdown();

	// We have to introduce this as variables for some reason. If we don't do that
	// we get strange division result. Why? I don't know!
	constant highest_mark = temporary_allocator.get_highest_mark();
	constant reserved_mem = CON_TEMPORARY_STORAGE_RESERVED_MEMORY;
	constant percent_value = static_cast<s32>( 100 * static_cast<f32>( highest_mark ) / reserved_mem );

	con_log( "Highest TA mark: % / % KB (~% percent).", highest_mark / CON_KILOBYTES( 1 ), reserved_mem / CON_KILOBYTES( 1 ), percent_value );

	constant current_time = Time::now();
	constant start_time   = Context.startup_time_point;
	constant difference   = Time::difference( start_time, current_time );
	constant playtime_in_minutes = Time::to_seconds( difference ) / 60;

	con_log( "Play time was % minutes.", playtime_in_minutes );

	con_pop_indent();
	flush_logger(); // flushing last messages here...

	main_logger.shutdown();
	default_allocator.shutdown();
	stack_allocator.shutdown();
}

void Application::flush_logger()
{
	FILE* main_logger_file = nullptr;
	main_logger_file = fopen( CON_DEFAULT_LOG_FILE, "wb" );

	if ( main_logger_file == nullptr ) {
		con_log( "Couldn't open log file \"%\"!", CString{ CON_DEFAULT_LOG_FILE } );
		main_logger_file = fopen( "fallback_log.txt", "wb" );
	} else {
		con_log( "Logger file successfully created." );
		con_log( "See ya later, aligater! :-)" );
	}

	// @ToDo: Check if we're in release mode. If yes, don't try to log to console.
	constant data_to_log = main_logger.get_buffer();
	if ( data_to_log.size == 0 ) {
		return;
	}

	// We had an accident with \0 once, so just in case...
#ifdef CON_DEBUG
	for ( s32 i = 0; i < data_to_log.size; ++i ) {
		con_assert( data_to_log.data[i] != 0 );
	}
#endif

	fputs( data_to_log.data, stdout );
	if ( main_logger_file != nullptr ) {
		std::fputs( data_to_log.data, main_logger_file );
	}

	fclose( main_logger_file );
}

returning Application::set_up_log_folder() -> bool
{
	namespace fs = std::filesystem;

	std::error_code fs_error_code;
	constant folder_exists = fs::exists( CON_LOGS_FOLDER, fs_error_code );

	if ( fs_error_code ) {
		con_log( R"(Error: couldn't check if logs folder at "%" exists. Info: "%".)", CString{ CON_LOGS_FOLDER }, cstring_from_stdstring( fs_error_code.message() ) );
		return false;
	}

	if ( !folder_exists ) {
		con_log( R"(Logs folder is missing, creating new one at "%".)", CString{ CON_LOGS_FOLDER } );

		fs_error_code ={};
		fs::create_directory( CON_LOGS_FOLDER, fs_error_code );

		if ( fs_error_code ) {
			con_push_indent();
			con_log( R"(Error: couldn't create! Info: "%".)", cstring_from_stdstring( fs_error_code.message() ) );
			con_pop_indent();
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
			con_log( R"(Error: missing path "%".)", path );
			success = false;
		}
	};


	path_exists( CON_DATA_FOLDER );
	path_exists( CON_ASSETS_FOLDER );
	path_exists( CON_SCENES_FOLDER );
	path_exists( CON_TEXTURES_FOLDER );
	path_exists( CON_FONTS_FOLDER );
	path_exists( CON_SHADERS_FOLDER );

	path_exists( CON_ASSETS_CONFIG_FILE );
	path_exists( CON_PLANETS_CONFIG_FILE );

	path_exists( CON_DEFAULT_SCENE_RESOURCES_INFO_FILE );

	return success;
}
}