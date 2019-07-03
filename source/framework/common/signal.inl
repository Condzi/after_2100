/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template <typename ...TArgs>
auto Signal<TArgs...>::bond( Function&& function ) -> std::function<void()>
{
	functions.emplace_back( std::forward<Function>( function ) );

	size_t idx = functions.size()-1;

	return [&functions, idx] {
		functions.erase( functions.begin() + idx );
	}
}

template <typename ...TArgs>
void Signal<TArgs...>::emit( TArgs ...args )
{
	for ( auto& f : functions )
		f( std::forward<TArgs>( args )... );
}
}