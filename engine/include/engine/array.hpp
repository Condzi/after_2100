#pragma once

#include <engine/allocator.hpp>
#include <engine/typedefs.hpp>
#include <engine/context.hpp>
#include <engine/assert.hpp>

namespace con
{
template <typename T>
class Array final
{
public:
	Array() = default;

	void initialize( s32 the_size_, Allocator* allocator_ = Context.default_allocator );
	void shutdown();

	void shrink( s32 amount_of_elements_to_shrink );

	returning operator[]( s32 idx ) -> T&;
	returning operator[]( s32 idx ) const -> T const&;

	returning data() -> T*;
	returning data() const -> T const*;
	returning size() const -> s32;

private:
	Allocator* allocator = nullptr;
	T* begin = nullptr;
	s32 size_ = -1;
};

//
//	Definitions
//

template <typename T>
void Array<T>::initialize( s32 the_size_, Allocator* allocator_ )
{
	con_assert( the_size_ > 0 );
	con_assert( allocator_ != nullptr );
	con_assert( begin == nullptr ); // You have to shutdown the array first before reinitializing it.

	allocator = allocator_;
	size_ = the_size_;

	begin = reinterpret_cast<T*>( allocator->allocate( sizeof( T ) * size_ ) );
}

template<typename T>
void Array<T>::shutdown()
{
	if ( begin == nullptr || size_ < 0 ) {
		return;
	}

	allocator->free( reinterpret_cast<byte*>( begin ), sizeof( T ) * size_ );
	begin = nullptr;
	size_ = -1;
}

template <typename T>
void Array<T>::shrink( s32 amount_of_elements_to_shrink )
{
	con_assert( amount_of_elements_to_shrink <= size_ );

	size_ -= amount_of_elements_to_shrink;

	allocator->free( reinterpret_cast<byte*>( begin + size_ ), sizeof( T ) * amount_of_elements_to_shrink );
}

template<typename T>
returning Array<T>::operator[]( s32 idx ) -> T&
{
	con_assert( idx >= 0 );
	con_assert( idx < size_ );

	return *( begin + idx );
}

template<typename T>
returning Array<T>::operator[]( s32 idx ) const -> T const&
{
	con_assert( idx >= 0 );
	con_assert( idx < size_ );

	return *( begin + idx );
}

template<typename T>
returning Array<T>::data() -> T*
{
	return begin;
}

template <typename T>
returning Array<T>::data() const -> T const*
{
	return begin;
}

template<typename T>
returning Array<T>::size() const -> s32
{
	return size_;
}

}