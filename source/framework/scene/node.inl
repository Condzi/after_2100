/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

namespace con
{
template <typename TNode>
auto Node::attach() -> TNode* const
{
	static_assert( std::is_base_of_v<Node, TNode> );

	auto node_to_attach = TNode::instantiate();
	node_to_attach->parent_node = this;

	child_nodes.push_back( change_owner( node_to_attach ) );

	TNode* return_val = dynamic_cast<TNode*>( child_nodes.back().get() );

	if( return_val is nullptr )
	{
		engine_log_critical( "You forgot CLASS_DEF( class_ ) for this TNode." );
	}

	return return_val;
}

template <typename TNode>
auto Node::cast_to() -> TNode *const
{
	static_assert( std::is_base_of_v<Node, TNode> );

	auto casted = dynamic_cast<TNode*>( this );
	report_error_if( casted is nullptr )
	{
		engine_log_error( "Error casting \"{}\" => \"{}\".", get_class_name(), TNode::get_class_name_static() );
	}

	return casted;
}
}