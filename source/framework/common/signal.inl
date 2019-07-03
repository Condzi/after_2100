/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template <typename ...TArgs>
auto Signal<TArgs...>::connect( Function function ) -> Disconnector
{
	unique_id_counter++;
	functions[unique_id_counter] = function;

	return [this, pos = unique_id_counter] {
 		functions.erase( pos );
	};
}

template <typename ...TArgs>
void Signal<TArgs...>::emit( TArgs ...args )
{
	for ( auto& f_pair : functions ) {
		// @Hack: iteration over empty map causes crash. This fixes it:
		if ( functions.empty() ) return;
		f_pair.second( std::forward<TArgs>( args )... );
	}
}
}