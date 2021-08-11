#pragma once
#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

#include"my_uninitialized.h"
#include"my_exceptdef.h"
#include"my_algorithm.h"
#include"my_iterator.h"
#include"my_allocator.h"

namespace mystl {

	template<typename T, typename Alloc = allocator<T>>
	class vector {
	public:
		using value_type = T;
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = value_type*;
		using const_iterator = const value_type*;
		using reverse_iterator = mystl::reverse_iterator<iterator>;
		using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

		using data_allocator = Alloc;
		allocator_type get_allocator() { return data_allocator(); }
	private:
		iterator begin_; //已使用的空间头
		iterator end_;   //已使用的空间尾后
		iterator cap_;   //已分配的空间尾后
	public:
		vector(); //默认构造
		explicit vector(size_type count); //count个默认值
		vector(size_type count, const value_type& val); //count个val值
		template<class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		vector(InputIt first, InputIt last); //范围构造
		vector(std::initializer_list<T> init); //初始化列表
		vector(const vector& other); //拷贝构造
		vector(vector&& other) noexcept; //移动构造
		~vector();

		vector& operator=(const vector& other);
		vector& operator=(vector&& other) noexcept;
		vector& operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T& value);
		template< class InputIt, typename std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		void assign(InputIt first, InputIt last);
		void assign(std::initializer_list<T> ilist);

	public:

		reference at(size_type n) { return (*this)[n]; }
		const_reference at(size_type n) const { return (*this)[n]; }
		reference operator[](size_type n) { ASSERT_EXPR(n < size()); return *(begin_ + n); }
		const_reference operator[](size_type n) const { ASSERT_EXPR(n < size()); return *(begin_ + n); }

		reference front() { ASSERT_EXPR(!empty()); return *begin_; }
		const_reference front() const { ASSERT_EXPR(!empty()); return *begin_; }
		reference back() { ASSERT_EXPR(!empty()); return *(end_ - 1); }
		const_reference back() const { ASSERT_EXPR(!empty()); return *(end_ - 1); }
		T* data() noexcept { return begin_; }
		const T* data() const noexcept { return begin_; }

		iterator begin() { return begin_; }
		iterator end() { return end_; }
		const_iterator begin() const { return begin_; }
		const_iterator end() const { return end_; }
		const_iterator cbegin() const { return begin_; }
		const_iterator cend() const { return end_; }
		reverse_iterator rbegin() noexcept { return reverse_iterator(end_); }
		reverse_iterator rend() noexcept { return reverse_iterator(begin_); }
		const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end_); }
		const_reverse_iterator rend() const noexcept { return  reverse_iterator(begin_); }
		const_reverse_iterator crbegin() const noexcept { return reverse_iterator(end_); }
		const_reverse_iterator crend() const noexcept { return  reverse_iterator(begin_); }



		bool empty() const { return begin_ == end_; }
		size_type size() const { return end_ - begin_; }
		void reserve(size_type new_cap);  //预分配
		size_type capacity() const { return cap_ - begin_; }
		void shrink_to_fit();           //移除未使用的容量（会使所有引用和迭代器失效）


		void clear() { erase(begin_, end_); };

		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);
		template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		iterator insert(const_iterator pos, InputIt first, InputIt last);
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);


		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);


		void push_back(const T& value);
		void push_back(T&& value);


		template< class... Args >
		void emplace_back(Args&&... args);


		void pop_back() {
			ASSERT_EXPR(!empty());
			mystl::destroy_at(end_ - 1);
			--end_;
		}

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(vector& other) noexcept;

	private:
		void reallocate(size_type new_cap) {
			auto new_begin = data_allocator::allocate(new_cap);
			auto new_end = mystl::uninitialized_move(begin_, end_, new_begin); //构造时begin_ = end_ = nullptr

			free();

			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + new_cap;
		}

		void reallocate() {
			//每次扩容1.5倍
			auto new_cap = size() <= 1 ? 2 : size() + (size() >> 1);
			//auto new_cap = size() == 0 ? 2 : size() * 2;
			reallocate(new_cap);
		}

		void fill_initialize(size_type count, const value_type& value) {
			reallocate(count);
			mystl::uninitialized_fill_n(begin_, count, value);
			end_ = begin_ + count;
		}

		template<typename InputIt>
		void range_initialize(InputIt first, InputIt last) {
			size_type count = mystl::distance(first, last);
			reallocate(count);
			mystl::uninitialized_copy(first, last, begin_);
			end_ = begin_ + count;
		}

		template<typename InputIt>
		void range_assign(InputIt first, InputIt last);

		void free() {
			if (begin_) {
				mystl::destroy(begin_, end_);
				data_allocator::deallocate(begin_, cap_ - begin_);
			}
		}
	};

	//构造函数
	template<typename T, typename Alloc>
	inline vector<T, Alloc>::vector()
		:begin_(nullptr),
		end_(nullptr),
		cap_(nullptr)
	{
	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>::vector(size_type count) :vector()
	{
		fill_initialize(count, value_type{});
	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>::vector(size_type count, const value_type& value) :vector()
	{
		fill_initialize(count, value);
	}

	template<typename T, typename Alloc>
	template<class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline vector<T, Alloc>::vector(InputIt first, InputIt last) :vector()
	{
		range_initialize(first, last);
	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>::vector(std::initializer_list<T> init) :vector()
	{
		range_initialize(init.begin(), init.end());
	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>::vector(const vector& other) :vector()
	{
		range_initialize(other.begin_, other.end_);
	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>::vector(vector&& other) noexcept
		:begin_(other.begin_),
		end_(other.end_),
		cap_(other.cap_)
	{
		other.begin_ = nullptr;
		other.end_ = nullptr;
		other.cap_ = nullptr;
	}

	//析构函数
	template<typename T, typename Alloc>
	inline vector<T, Alloc>::~vector()
	{
		free();
	}

	//用[first,last)中的元素赋值给自身
	//自赋值不会出现问题
	template<typename T, typename Alloc>
	template<typename InputIt>
	void vector<T, Alloc>::range_assign(InputIt first, InputIt last)
	{
		size_type new_size = mystl::distance(first, last);
		//容量足够
		if (new_size <= capacity()) {
			if (size() >= new_size) {  //现有元素数量大于等于待赋值元素数量
				auto new_end = mystl::copy(first, last, begin_);
				mystl::destroy(new_end, end_);
				end_ = new_end;
			}
			else { //现有元素数量小于待赋值元素数量
				auto mid = first;
				mystl::advance(mid, size());
				mystl::copy(first, mid, begin_); //初始化过的区域
				end_ = mystl::uninitialized_copy(mid, last, end_);//未初始化的区域
			}
		}
		else {  //容量不足
			vector tmp(first, last);
			swap(tmp);
		}
	}

	//operator=
	template<typename T, typename Alloc>
	inline vector<T, Alloc>&
		vector<T, Alloc>::operator=(const vector& other) {
		if (this != &other) {
			range_assign(other.begin(), other.end());
		}
		return *this;
	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>&
		vector<T, Alloc>::operator=(vector&& other) noexcept {
		if (this != &other) {
			free();
			begin_ = other.begin_;
			end_ = other.end_;
			cap_ = other.cap_;
			other.begin_ = nullptr;
			other.end_ = nullptr;
			other.cap_ = nullptr;
		}
		return *this;

	}

	template<typename T, typename Alloc>
	inline vector<T, Alloc>&
		vector<T, Alloc>::operator=(std::initializer_list<T> ilist) {
		range_assign(ilist.begin(), ilist.end());
		return *this;
	}

	//assign
	template<typename T, typename Alloc>
	void vector<T, Alloc>::assign(size_type count, const T& value)
	{
		//容量足够
		if (count <= capacity()) {
			if (size() > count) {//现有元素数量大于待赋值元素数量
				auto new_end = mystl::fill_n(begin(), count, value);
				mystl::destroy(new_end, end_);
				end_ = new_end;
			}
			else {//现有元素数量小于等于待赋值元素数量
				end_ = mystl::fill_n(begin(), count, value);
			}
		}
		else { //容量不足
			vector tmp(count, value);
			swap(tmp);
		}
	}

	template<typename T, typename Alloc>
	template<class InputIt, typename std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void vector<T, Alloc>::assign(InputIt first, InputIt last)
	{
		range_assign(first, last);
	}

	template<typename T, typename Alloc>
	void assign(std::initializer_list<T> ilist) {
		range_assign(ilist.begin(), ilist.end());
	}


	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::reserve(size_type new_cap) {
		if (new_cap > capacity()) {
			reallocate(new_cap);
		}
	}

	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::shrink_to_fit() {
		if (end_ < cap_) {
			reallocate(size());
		}
	}

	//insert
	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(const_iterator pos, const T& value)
	{
		return emplace(pos, value);
	}

	template<typename T, typename Alloc>
	inline typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(const_iterator pos, T&& value)
	{
		return emplace(pos, std::move(value));
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(const_iterator pos, size_type count, const T& value)
	{
		ASSERT_EXPR(pos >= begin_ && pos <= end_);
		if (count == 0)
			return const_cast<iterator>(pos);
		if (static_cast<size_type>(cap_ - end_) < count) {
			//避免扩容引发迭代器失效，提前记录下标
			size_type pos_index = pos - begin_;
			reallocate(size() + count);
			pos = begin_ + pos_index;
		}
		//先将元素添加到尾部，然后旋转到指定位置
		mystl::uninitialized_fill_n(end_, count, value);
		end_ += count;
		iterator rotate_first = const_cast<iterator>(pos);
		mystl::rotate(rotate_first, end_ - count, end_);

		return rotate_first;
	}

	template<typename T, typename Alloc>
	template<class InputIt, typename std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(const_iterator pos, InputIt first, InputIt last)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end() && first <= last);
		if (first == last)
			return const_cast<iterator>(pos);
		
		size_type count = mystl::distance(first, last);
		if (static_cast<size_type>(cap_ - end_) < count) {
			//避免扩容引发迭代器失效，提前记录下标
			size_type pos_index = pos - begin_;
			//从自己插入
			if (first >= begin() && first <= end()) {
				size_type first_index = first - begin_;
				reallocate(size() + count);
				first = begin_ + first_index;
			}
			else {
				reallocate(size() + count);
			}
			pos = begin_ + pos_index;
		}
		//先将元素添加到尾部，然后旋转到指定位置
		mystl::uninitialized_copy(first, first + count, end_);
		end_ += count;
		iterator rotate_first = const_cast<iterator>(pos);
		mystl::rotate(rotate_first, end() - count, end());

		return rotate_first;
	}

	template<typename T, typename Alloc>
	inline typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist) {
		return insert(pos, ilist.begin(), ilist.end());
	}


	template<typename T, typename Alloc>
	template<class ...Args>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::emplace(const_iterator pos, Args && ...args)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end());

		//在end_处插入
		if (pos == end_) {
			if (end_ == cap_) {
				reallocate();
			}
			mystl::construct(end_, std::forward<Args>(args)...);
			++end_;
		}
		else { //不在end_处插入
			if (end_ == cap_) {
				auto pos_index = pos - begin_;
				reallocate();
				pos = begin_ + pos_index;
			}
			//在尾部构造，然后旋转到指定位置
			mystl::construct(end_, std::forward<Args>(args)...);
			++end_;

			iterator rotate_first = const_cast<iterator>(pos);
			mystl::rotate(rotate_first, end_ - 1, end_);
		}

		return const_cast<iterator>(pos);
	}


	//erase
	template<typename T, typename Alloc>
	inline typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(const_iterator pos) 
	{
		ASSERT_EXPR(pos >= begin() && pos < end());
		iterator d_first = const_cast<iterator>(pos);
		end_ = mystl::move(d_first + 1, end_, d_first);
		mystl::destroy_at(end_);
		return d_first;
	}

	//移除范围 [first, last) 中的元素
	template<typename T, typename Alloc>
	inline typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(const_iterator first, const_iterator last) 
	{
		ASSERT_EXPR(first >= begin() && last <= end() && first <= last);

		iterator old_end = end_;
		iterator d_first = const_cast<iterator>(first);
		end_ = mystl::move(const_cast<iterator>(last), end_, d_first);
		mystl::destroy(end_, old_end);
		return d_first;
	}

	//push_back
	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::push_back(const T& value)
	{
		if (end_ != cap_) {
			mystl::construct(end_, value);
			++end_;
		}
		else {
			emplace(end_, value);
		}
	}

	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::push_back(T&& value)
	{
		if (end_ != cap_) {
			mystl::construct(end_, std::move(value));
			++end_;
		}
		else {
			emplace(end_, std::move(value));
		}
	}

	template<typename T, typename Alloc>
	template<class ...Args>
	inline void vector<T, Alloc>::emplace_back(Args && ...args)
	{
		if (end_ != cap_) {
			mystl::construct(end_, std::forward<Args>(args)...);
			++end_;
		}
		else {
			emplace(end_, std::forward<Args>(args)...);
		}
	}

	//resize
	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::resize(size_type count)
	{
		resize(count, value_type{});
	}

	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::resize(size_type count, const value_type& value)
	{
		if (count > size()) {
			insert(end(), count - size(), value);
		}
		else if (count < size()) {
			erase(begin() + count, end());
		}
	}

	template<typename T, typename Alloc>
	inline void vector<T, Alloc>::swap(vector& other) noexcept
	{
		if (this != &other) {
			mystl::swap(begin_, other.begin_);
			mystl::swap(end_, other.end_);
			mystl::swap(cap_, other.cap_);
		}
	}


	//非成员函数
	template< class T, class Alloc>
	bool operator==(const vector<T, Alloc>& lhs,
		const vector<T, Alloc>& rhs) {
		return lhs.size() == rhs.size() &&
			mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}
	template< class T, class Alloc >
	bool operator!=(const vector<T, Alloc>& lhs,
		const vector<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template< class T, class Alloc >
	bool operator<(const vector<T, Alloc>& lhs,
		const vector<T, Alloc>& rhs) {
		return mystl::lexicographical_compare(
			lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}

	template< class T, class Alloc >
	bool operator<=(const vector<T, Alloc>& lhs,
		const vector<T, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template< class T, class Alloc >
	bool operator>(const vector<T, Alloc>& lhs,
		const vector<T, Alloc>& rhs) {
		return rhs < lhs;
	}

	template< class T, class Alloc >
	bool operator>=(const vector<T, Alloc>& lhs,
		const vector<T, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template< class T, class Alloc >
	void swap(vector<T, Alloc>& lhs,
		vector<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}
} //end of namespace mystl

#endif // !_MY_VECTOR_H_
