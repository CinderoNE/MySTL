#pragma once
#ifndef _MY_CONSTRUCT_H_
#define _MY_CONSTRUCT_H_
namespace mystl {

	template<typename T,typename... Args>
	void construct(T* ptr, Args&&... args) {
		::new (ptr) T(std::forward<Args>(args)...);
	}

	template< class T >
	inline void destroy_at(T* p) {
		if(p != nullptr)
			p->~T();  //标准类型执行伪析构函数调用 ，没有影响
	}

	template< class ForwardIt >
	void destroy(ForwardIt first, ForwardIt last) {
		for (; first != last; ++first)
			destroy_at(std::addressof(*first));
	}
} // end of namespace mystl

#endif // !_MY_CONSTRUCT_H_
