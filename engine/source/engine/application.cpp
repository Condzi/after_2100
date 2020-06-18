#include <engine/application.hpp>

#include <filesystem>

namespace con
{
returning Application::initialize() -> bool
{
	Context.default_allocator = &default_allocator;
	Context.temporary_allocator = &temporary_allocator;
	Context.entity_manager = &entity_manager;
	Context.logger = &main_logger;
	Context.config_file = &config_file;

	default_allocator.initialize();
	temporary_allocator.initialize();
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
	flush_logger();

	// @Robustness: I'm fired. It's a stupid-ass idea to not return a bool
	// here. 
	// @ToDo: Fallback to default values somehow. Maybe hold struct of const
	// references / values isntead strings.
	config_file.parse( CON_CONFIG_FILE );
	con_log_indented( 1, "(FIXME) Probably done loading config file. IDK because I'm fired" );

	// @ToDo: Load config here and fallback to default values if necessary
	// @ToDo: Splash screen stuff?? in separate thread? use it 
	// @ToDo: Initialize window here.
	// @ToDo: Load resources here.

	con_log( "Initialization completed." );
	flush_logger();
	return true;
}
void Application::run()
{
	con_log( "Application runs..." );

	// @ToDo: while(Context.flags.exit_flags.all_unset() == true ) ?
	return;
}

void Application::shutdown()
{
	flush_logger();
	con_log( "Application shutdown..." );
	config_file.free();
	// @ToDo: entity_manager.shutdown();
	// @ToDo: window.shutdown()...
	flush_logger(); // flushing last messages here...
	main_logger.shutdown();
	std::fclose( main_logger_file );
	default_allocator.shutdown();
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
	path_exists( CON_CONFIG_FILE );

	return success;
}
}