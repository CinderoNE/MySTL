#pragma once
#ifndef _MY_ITERATOR_H_
#define _MY_ITERATOR_H_


#include<cstddef>
#include"my_type_traits.h"

namespace mystl {
	//标准5中迭代器

	//输入迭代器是可用于顺序输入操作的迭代器，其中迭代器指向的每个值只读取一次，然后迭代器递增。
	//所有前向、双向和随机访问迭代器也是有效的输入迭代器。
	struct input_iterator_tag {};

	//输出迭代器是可用于顺序输出操作的迭代器，其中迭代器指向的每个元素只写入一次值，然后迭代器递增。
	//所有不是常量迭代器的前向、双向和随机访问迭代器也是有效的输出迭代器。
	struct output_iterator_tag {};

	struct forward_iterator_tag : input_iterator_tag {};

	struct bidirectional_iterator_tag : forward_iterator_tag {};

	struct random_access_iterator_tag : bidirectional_iterator_tag {};

	template <typename Category, typename T, typename Distance = ptrdiff_t,
		typename Pointer = T*, typename Reference = T&>
		struct iterator
	{
		using iterator_category = Category;
		using value_type = T;
		using pointer = Pointer;
		using reference = Reference;
		using difference_type = Distance;
	};

	/*==============================================*/
	//迭代器萃取机
	template <typename, typename = void>
	struct iterator_traits {}; //非迭代器类型

	
	template<typename Iterator>
	struct iterator_traits<Iterator,
		std::void_t<typename Iterator::iterator_category, typename Iterator::value_type, 
		typename Iterator::difference_type,typename Iterator::pointer, typename Iterator::reference>>
	{
		using iterator_category = typename Iterator::iterator_category;
		using value_type = typename Iterator::value_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
		using difference_type = typename Iterator::difference_type;
	};

	//原生指针的萃取机
	template<typename T>
	struct iterator_traits<T*> {
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = ptrdiff_t;
	};

	template<typename T>
	struct iterator_traits<const T*> {
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using difference_type = ptrdiff_t;
	};

	/*==============================================*/
	//萃取单独属性
	template<typename Iterator>
	using iter_value_t = typename iterator_traits<Iterator>::value_type;

	template<typename Iterator>
	using iter_ref_t = typename iterator_traits<Iterator>::reference;

	template <typename Iterator>
	using iter_diff_t = typename iterator_traits<Iterator>::difference_type;

	template <typename Iterator>
	using iter_cat_t = typename iterator_traits<Iterator>::iterator_category;

	/*==============================================*/
	//是否是迭代器类型
	template<typename T ,typename = void>
	constexpr bool is_iterator_v = false;

	template<typename T>
	constexpr bool is_iterator_v<T, std::void_t<iter_cat_t<T>>> = true;

	/*==============================================*/
	//判断是否是某种类型迭代器
	template <typename Iterator,typename = void>
	constexpr bool is_input_iter_v = false;

	template <typename Iterator>
	constexpr bool is_input_iter_v<Iterator, std::void_t<iter_cat_t<Iterator>>> 
		= mystl::is_convertible_v<iter_cat_t<Iterator>, input_iterator_tag>;

	template <typename Iterator, typename = void>
	constexpr bool is_fwd_iter_v = false;

	template <typename Iterator>
	constexpr bool is_fwd_iter_v<Iterator, std::void_t<iter_cat_t<Iterator>>> 
		= mystl::is_convertible_v<iter_cat_t<Iterator>, forward_iterator_tag>;

	template <typename Iterator, typename = void>
	constexpr bool is_bidi_iter_v = false;

	template <typename Iterator>
	constexpr bool is_bidi_iter_v<Iterator, std::void_t<iter_cat_t<Iterator>>> 
		= mystl::is_convertible_v<iter_cat_t<Iterator>, bidirectional_iterator_tag>;

	template <typename Iterator, typename = void>
	constexpr bool is_random_iter_v = false;

	template <typename Iterator>
	constexpr bool is_random_iter_v<Iterator, std::void_t<iter_cat_t<Iterator>>> 
		= mystl::is_convertible_v<iter_cat_t<Iterator>, random_access_iterator_tag>;

	/*==============================================*/
	//distance
	template<typename InputIterator>
	inline iter_diff_t<InputIterator> 
	__distance(InputIterator first, InputIterator last,input_iterator_tag) {
		iter_diff_t<InputIterator> n = 0;
		while (first != last) {
			++first;
			++n;
		}
		return n;
	}

	template<typename RandomAccessIterator>
	inline iter_diff_t<RandomAccessIterator> 
	__distance(RandomAccessIterator first, RandomAccessIterator last,random_access_iterator_tag) {
		return last - first;
	}

	template<typename Iterator>
	inline iter_diff_t<Iterator> 
	distance(Iterator first, Iterator last) {
		return __distance(first, last, iter_cat_t<Iterator>{});
	}

	/*==============================================*/
	//advance
	template<typename InputIterator,typename Distance>
	inline void 
	__advance(InputIterator& iter, Distance n, input_iterator_tag) {
		while (n--)
			++iter;
	}

	template<typename BidirectionalIterator, typename Distance>
	inline void 
	__advance(BidirectionalIterator& iter, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--) ++iter;
		else
			while (n++) --iter;
	}

	template<typename RandomAccessIterator, typename Distance>
	inline void
	__advance(RandomAccessIterator& iter, Distance n, random_access_iterator_tag) {
		iter += n;
	}

	template<typename Iterator,typename Distance>
	inline void advance(Iterator& iter, Distance n) {
		__advance(iter, n, iter_cat_t<Iterator>());
	}


	/*==============================================*/

	//reverse_iterator
	template <class Iterator>
	class reverse_iterator
	{
	public:
		using iterator_type = Iterator;

		using iterator_category = iter_cat_t<Iterator>;
		using value_type = iter_value_t<Iterator>;
		using difference_type = iter_diff_t<Iterator>;
		using pointer = typename iterator_traits<Iterator>::pointer;
		using reference = iter_ref_t<Iterator>;

		using self =  reverse_iterator<Iterator>;

	private:
		Iterator current;  // 正向迭代器类型
	public:
		// 构造函数
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) :current(i) {}
		reverse_iterator(const self& rhs) :current(rhs.current) {}

	public:
		// 取出对应的正向迭代器
		iterator_type base() const
		{
			return current;
		}

		// 重载操作符
		reference operator*() const
		{ // 实际对应正向迭代器的前一个位置
			auto tmp = current;
			return *--tmp;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		// 前进(++)变为后退(--)
		self& operator++()
		{
			--current;
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			--current;
			return tmp;
		}
		// 后退(--)变为前进(++)
		self& operator--()
		{
			++current;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		self& operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}
		self operator+(difference_type n) const
		{
			return self(current - n);
		}
		self& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}
		self operator-(difference_type n) const
		{
			return self(current + n);
		}

		reference operator[](difference_type n) const
		{
			return *(*this + n);
		}
	};

	// 重载 operator-
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}

	// 重载比较操作符
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() < lhs.base();
	}

	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return rhs < lhs;
	}

	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs < rhs);
	}

} //end of namespace mystl


#endif // !_MY_ITERATOR_H_
