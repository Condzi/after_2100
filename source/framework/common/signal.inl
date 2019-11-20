/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template<typename ...TArgs>
Signal<TArgs...>::Signal()
{
	functions.reserve( RESERVED );
}

template <typename ...TArgs>
auto Signal<TArgs...>::connect( Function function ) -> Disconnector
{
	unique_id_counter++;
	functions.emplace_back( function );

	return[this, pos = unique_id_counter-1]{
		// just reset it.
		// @ToDo: Waste of memory since we are not removing it.
		// If we would remove it, we would invalidate the iterator
		functions[pos] = Function{};
	};
}

template <typename ...TArgs>
void Signal<TArgs...>::emit( TArgs ...args )
{
	G_Profile_Scope( "Signal::emit" );
	for ( auto& func : functions )
		if ( func )
			func( std::forward<TArgs>( args )... );
}
}