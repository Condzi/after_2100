/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{

template<typename ...TArgs>
inline Signal<TArgs...>::Subscriber::Subscriber( Node* node, Function&& f ) :
	bonded_node( node ),
	function_to_call( std::forward<Function>( f ) )
{}

template <typename ...TArgs>
void Signal<TArgs...>::bond( Node* node, Function&& function )
{
	subscribers.emplace_back( node, std::forward<Function>( function ) );
}

template <typename ...TArgs>
void Signal<TArgs...>::notify( TArgs ...args )
{
	for ( auto& sub : subscribers )
		sub.function_to_call( std::forward<TArgs>( args )... );
}

template <typename ...TArgs>
void Signal<TArgs...>::remove_invalid_subscribers()
{
	remove_if( subscribers,
			   []( constant& subscriber ) {
				   return subscriber.bonded_node->is_queued_for_delete();
			   } );
}
}