#pragma once
#ifndef _MY_ALLOCTOR_H_
#define _MY_ALLOCTOR_H_

#include<cstddef> //ptrdiff_t
#include<new> //operator new
#include"my_construct.h"

namespace mystl {
	template<typename T>
	class allocator {
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		
	public:
		static T* allocate(const size_type count);

		static void deallocate(T* const ptr, const size_type count);

	};

	template<typename T>
	inline T* allocator<T>::allocate(const size_type count)
	{
		if (count == 0)
			return nullptr;
		return static_cast<T*>(::operator new(sizeof(T) * count));
	}

	template<typename T>
	inline void allocator<T>::deallocate(T* const ptr, const size_type)
	{
		if (ptr != nullptr)
			::operator delete(ptr);
	}

} //end of namespace mystl

#endif //_ALLOCTOR_H_