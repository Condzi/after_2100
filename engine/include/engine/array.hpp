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
	explicit Array( s32 the_size_, Allocator* allocator_ = Context.default_allocator );
	~Array();

	returning operator[]( s32 idx ) -> T&;
	returning operator[]( s32 idx ) const -> T const&;

	returning size() const -> s32;

private:
	Allocator* allocator = nullptr;
	T* begin = nullptr;
	s32 size_ = -1;
};

//
//	Definitions
//

template<typename T>
Array<T>::Array( s32 the_size_, Allocator* allocator_ )
{
	con_assert( the_size_ > 0 );
	con_assert( allocator_ != nullptr );

	allocator = allocator_;
	size_ = the_size_;

	begin = reinterpret_cast<T*>( allocator->allocate( size_ ) );
}

template<typename T>
Array<T>::~Array()
{
	allocator->free( begin, size_ );
}

template<typename T>
returning Array<T>::operator[]( s32 idx ) -> T&
{
	con_assert( idx > 0 );
	con_assert( idx < size_ );

	return begin + idx;
}

template<typename T>
returning Array<T>::operator[]( s32 idx ) const -> T const&
{
	con_assert( idx > 0 );
	con_assert( idx < size_ );

	return begin + idx;
}

template<typename T>
returning Array<T>::size() const -> s32
{
	return size_;
}

}