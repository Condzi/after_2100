/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

namespace con
{
template<typename ...TArgs>
inline void GUI_String::update_locale_parameters( TArgs ...args )
{
	constant wstring = localized_string.get_localized_text().toWideString();

	try {
		string = fmt::format( wstring, std::forward<TArgs>( args )... );
	}
	catch ( std::exception const& ex ) {
		engine_log_error( "Error formating locale string: \"{}\".", ex.what() );
		string = wstring;
	}

}
}