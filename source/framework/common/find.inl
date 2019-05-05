/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template <typename TContainer, typename TValue>
Find_Result<TValue> find( TContainer const& container, TValue const& value )
{
	calculation_constant begin = container.begin();
	calculation_constant end = container.end();
	calculation_constant result = std::find( begin, end, value );

	if ( result == end )
		return Find_Result<TValue>();

	s32 idx = static_cast<s32>( std::distance( begin, result ) );
	TValue const *const value = &( *result );

	return { true, idx, value };
}

template <typename TValue, template<typename> class TContainer, typename TLambda>
Find_Result<TValue> find_if( TContainer<TValue> const& container, TLambda&& lambda )
{
	calculation_constant begin = container.begin();
	calculation_constant end = container.end();
	calculation_constant result = std::find_if( begin, end, change_owner( lambda ) );

	if ( result == end )
		return Find_Result<TValue>();

	s32 idx = static_cast<s32>( std::distance( begin, result ) );
	TValue const *const value = &( *result );

	return { true, idx, value };
}
}