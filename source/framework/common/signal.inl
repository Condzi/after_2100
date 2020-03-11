/*
	Conrad 'Condzi' Kubacki 2020
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
auto Signal<TArgs...>::connect( Function function ) -> Signal_Disconnector
{
	unique_id_counter++;
	functions.emplace_back( function );

	return[this, pos = unique_id_counter-1]{

		if ( functions.empty() or pos > functions.size() ) {
			// Internal compiler error?
			// Probably wrong disconnector is bonded.
				std::cout<< "Signal is empty but still has destructor for id " << pos << std::endl;
			} else
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
	for ( s32 i = 0; i < functions.size(); i++ )
		if ( functions[i] )
			functions[i]( std::forward<TArgs>( args )... );
}
}