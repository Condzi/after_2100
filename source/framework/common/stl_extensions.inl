/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template <typename TContainer, typename TValue>
Find_Result find( TContainer const& container, TValue const& value )
{
	constant begin = container.begin();
	constant end = container.end();
	constant result = std::find( begin, end, value );

	if ( result == end )
		return {};

	s32 idx = static_cast<s32>( std::distance( begin, result ) );

	return { true, idx };
}

template <typename TContainer, typename TLambda>
Find_Result find_if( TContainer const& container, TLambda&& lambda )
{
	constant begin = container.begin();
	constant end = container.end();
	constant result = std::find_if( begin, end, change_owner( lambda ) );

	if ( result == end )
		return {};

	s32 idx = static_cast<s32>( std::distance( begin, result ) );

	return { true, idx };
}

template <typename TContainer, typename TLambda>
void remove_if( TContainer& container, TLambda&& lambda )
{
	constant begin = container.begin();
	constant end = container.end();

	container.erase( std::remove_if( begin, end, change_owner( lambda ) ), end );
}
}