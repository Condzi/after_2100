/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template <typename ...TArgs>
auto Signal<TArgs...>::connect( Function&& function ) -> std::function<void()>
{
	unique_id_counter++;
	functions[unique_id_counter] = function;
	
	return [&functions, pos = unique_id_counter] {
		functions.erase( pos );
	}
}

template <typename ...TArgs>
void Signal<TArgs...>::emit( TArgs ...args )
{
	for ( auto& f : functions )
		f( std::forward<TArgs>( args )... );
}
}