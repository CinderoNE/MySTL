#pragma once
#ifndef _MY_CONSTRUCT_H_
#define _MY_CONSTRUCT_H_

#include"my_type_traits.h"
#include"my_iterator.h"

namespace mystl {

	template<typename T,typename... Args>
	void construct(T* ptr, Args&&... args) {
		::new (ptr) T(std::forward<Args>(args)...);
	}

	template<typename T>
	inline void __destroy_at(T* p, mystl::false_type) {
		if (p != nullptr)
			p->~T();
	}

	template<typename T>
	inline void __destroy_at(T* p, mystl::true_type) {}

	template<typename T>
	inline void destroy_at(T* p) {
		__destroy_at(p, mystl::is_trivially_destructible<T>());
	}

	template<typename ForwardIt>
	inline void __destroy(ForwardIt first, ForwardIt last, mystl::false_type) {
		for (; first != last; ++first)
			mystl::destroy_at(std::addressof(*first));
	}

	template<typename ForwardIt>
	inline void __destroy(ForwardIt first, ForwardIt last, mystl::true_type) {}

	template<typename ForwardIt>
	void destroy(ForwardIt first, ForwardIt last) {
		using value_type = mystl::iter_value_t<ForwardIt>;
		__destroy(first, last, mystl::is_trivially_destructible<value_type>());
	}
} // end of namespace mystl

#endif // !_MY_CONSTRUCT_H_
