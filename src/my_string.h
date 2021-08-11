#pragma once
#ifndef _MY_basic_string_H_
#define _MY_basic_string_H_

#include"my_allocator.h"
#include"my_exceptdef.h"
#include"my_algorithm.h"

#include <cstddef> //for ptrdiff_t
#include<iostream> //for ostream
#include<cstring>

namespace mystl {

	/*
	只针对char类型实现了basic_string，其他字符类型无法使用
	*/
	template<typename CharT, typename Alloc = allocator<CharT>>
	class basic_string {
	public:
		using value_type = CharT;
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
	public:
		//末尾
		static constexpr size_type npos = static_cast<size_type>(-1);
	private:
		static constexpr size_type kInitBufferSize = 16;  //初始化大小，不从堆中分配
		static constexpr size_type kInitCapacity = kInitBufferSize - 1; //-1是因为末尾的'/0'
	private:
		size_type size_;  //当前字符数
		size_type cap_;   //当前分配的存储容量
		value_type init_buffer_[kInitBufferSize]{ '\0' };  //small basic_basic_string optimization
		iterator buffer_; //字符串开头
	private:
		void chooseBuffer();
		void reallocate(size_type new_size);
		size_type ge_near_pow2(size_type n);
		void free();

	public:
		basic_string();
		basic_string(size_type count, CharT ch);
		basic_string(const basic_string& other);
		basic_string(const basic_string& other, size_type pos, size_type count = npos);
		basic_string(const CharT* cstr);
		basic_string(const CharT* cstr, size_type count);
		template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		basic_string(InputIt first, InputIt last);
		basic_string(basic_string&& other) noexcept;
		basic_string(std::initializer_list<CharT> ilist);
		~basic_string();


		basic_string& operator=(const basic_string& str);
		basic_string& operator=(const_pointer cstr);
		basic_string& operator=(value_type ch);
		basic_string& operator=(basic_string&&) noexcept;

		basic_string& assign(const basic_string& str);
		basic_string& assign(const_pointer cstr);
		basic_string& assign(iterator first, iterator last);

		//元素访问
		reference at(size_t pos) { return operator[](pos); }
		const_reference at(size_t pos) const { return operator[](pos); }
		reference operator[](size_type pos) {
			ASSERT_EXPR(pos < size_);
			return buffer_[pos];
		}
		const_reference operator[](size_type pos) const {
			ASSERT_EXPR(pos < size_);
			return buffer_[pos];
		}
		reference front() { ASSERT_EXPR(!empty()); return buffer_[0]; }
		const const_reference front() const { ASSERT_EXPR(!empty()); return buffer_[0]; }
		reference back() { ASSERT_EXPR(!empty()); return buffer_[size_ - 1]; }
		const const_reference back() const { ASSERT_EXPR(!empty()); return buffer_[size_ - 1]; }

		const_pointer data() const { buffer_[size_] = '\0'; return buffer_; }
		const_pointer c_str() const { buffer_[size_] = '\0'; return buffer_; }

		//迭代器
		iterator begin() noexcept { return buffer_; }
		const_iterator begin() const noexcept { return buffer_; }
		const_iterator cbegin() const noexcept { return buffer_; }
		iterator end() noexcept{ return buffer_ + size_; }
		const_iterator end() const noexcept { return buffer_ + size_; }
		const_iterator cend() const noexcept { return buffer_ + size_; }

		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
		const_reverse_iterator crbegin() const noexcept { return reverse_iterator(end()); }
		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator crend() const noexcept { return reverse_iterator(begin()); }

		//容量
		bool empty() const { return size_ == 0; }
		size_type size() const { return size_; }
		size_type length() const { return size_; }
		void reserve(size_type new_cap = 0);
		size_type capacity() const { return cap_; }
		void shrink_to_fit();

		//操作
		void clear() { size_ = 0; }

		basic_string& insert(size_type index, size_type count, CharT ch);
		basic_string& insert(size_type index, const CharT* s);
		basic_string& insert(size_type index, const CharT* s, size_type count);
		basic_string& insert(size_type index, const basic_string& str);
		basic_string& insert(size_type index, const basic_string& str,size_type index_str, size_type count = npos);

		iterator insert(const_iterator pos, CharT ch);
		iterator insert(const_iterator pos, size_type count, CharT ch);
		template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		iterator insert(const_iterator pos, InputIt first, InputIt last);
		iterator insert(const_iterator pos, std::initializer_list<CharT> ilist);

		basic_string& erase(size_type index = 0, size_type count = npos);
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		void push_back(value_type ch) { append(1, ch); }
		void pop_back() { ASSERT_EXPR(!empty()); --size_; }
		
		basic_string& append(size_type count, CharT ch);
		basic_string& append(const basic_string& str);
		basic_string& append(const basic_string& str, size_type pos, size_type count = npos);
		basic_string& append(const CharT* cstr, size_type count);
		basic_string& append(const CharT* cstr);
		template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		basic_string& append(InputIt first, InputIt last);
		basic_string& append(std::initializer_list<CharT> ilist);

		basic_string& operator+=(const basic_string& str);
		basic_string& operator+=(CharT ch);
		basic_string& operator+=(const CharT* cstr);
		basic_string& operator+=(std::initializer_list<CharT> ilist);
		
		int compare(const basic_string& str) const noexcept { return strcmp(c_str(), str.c_str()); }
		int compare(size_type pos1, size_type count1, const basic_string& str) const;
		int compare(size_type pos1, size_type count1,const basic_string& str,size_type pos2, size_type count2 = npos) const;
		int compare(const CharT* s) const { return strcmp(c_str(), s); }
		int compare(size_type pos1, size_type count1,const CharT* s) const;
		int compare(size_type pos1, size_type count1,const CharT* s, size_type count2) const;

		//TODO..
		basic_string& replace(size_type pos, size_type count,const basic_string& str);
		basic_string& replace(const_iterator first, const_iterator last,const basic_string& str);
		basic_string& replace(size_type pos, size_type count,const basic_string& str,size_type pos2, size_type count2 = npos);
		template< class InputIt , std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		basic_string& replace(const_iterator first, const_iterator last,InputIt first2, InputIt last2);
		basic_string& replace(size_type pos, size_type count,const CharT* cstr, size_type count2);
		basic_string& replace(const_iterator first, const_iterator last,const CharT* cstr, size_type count2);
		basic_string& replace(size_type pos, size_type count,const CharT* cstr);
		basic_string& replace(const_iterator first, const_iterator last,const CharT* cstr);
		basic_string& replace(size_type pos, size_type count,size_type count2, CharT ch);
		basic_string& replace(const_iterator first, const_iterator last,size_type count2, CharT ch);
		basic_string& replace(const_iterator first, const_iterator last,std::initializer_list<CharT> ilist);
		

		basic_string substr(size_type pos = 0, size_type count = npos) const;
		size_type copy(CharT* dest, size_type count, size_type pos = 0) const;

		void resize(size_type count);
		void resize(size_type count, CharT ch);

		void swap(basic_string& rhs) noexcept;

		//查找
		size_type find(const basic_string& str, size_type pos = 0) const noexcept;
		size_type find(const CharT* s, size_type pos, size_type count) const;
		size_type find(const CharT* s, size_type pos = 0) const;
		size_type find(CharT ch, size_type pos = 0) const noexcept;

		size_type rfind(const basic_string& str,size_type pos = npos) const noexcept;
		size_type rfind(const CharT* s, size_type pos, size_type count) const;
		size_type rfind(const CharT* s, size_type pos = npos) const;
		size_type rfind(CharT ch, size_type pos = npos) const noexcept;

		//寻找等于给定字符序列中字符之一的首个字符
		size_type find_first_of(const basic_string& str,size_type pos = 0) const noexcept;
		size_type find_first_of(const CharT* s, size_type pos, size_type count) const;
		size_type find_first_of(const CharT* s, size_type pos = 0) const;
		size_type find_first_of(CharT ch, size_type pos = 0) const noexcept;

		//寻找不等于给定字符序列中任何字符的首个字符
		size_type find_first_not_of(const basic_string& str,size_type pos = 0) const noexcept;
		size_type find_first_not_of(const CharT* s,size_type pos, size_type count) const;
		size_type find_first_not_of(const CharT* s,size_type pos = 0) const;
		size_type find_first_not_of(CharT ch, size_type pos = 0) const noexcept;

		//寻找等于给定字符序列中字符之一的最后字符,搜索只考虑区间 [0, pos]
		size_type find_last_of(const basic_string& str,size_type pos = npos) const noexcept;
		size_type find_last_of(const CharT* s,size_type pos, size_type count) const;
		size_type find_last_of(const CharT* s, size_type pos = npos) const;
		size_type find_last_of(CharT ch, size_type pos = npos) const noexcept;

		//寻找不等于给定字符序列中任何字符的最后字符,搜索只考虑区间 [0, pos] 
		size_type find_last_not_of(const basic_string& str,size_type pos = npos) const noexcept;
		size_type find_last_not_of(const CharT* s,size_type pos, size_type count) const;
		size_type find_last_not_of(const CharT* s, size_type pos = npos) const;
		size_type find_last_not_of(CharT ch, size_type pos = npos) const noexcept;
	};


	//函数定义
	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::chooseBuffer()
	{
		if (size_ <= kInitCapacity) {  //初始大小能存放下所有字符
			buffer_ = init_buffer_;
			cap_ = kInitCapacity;
		}
		else {
			reallocate(size_);
		}
	}

	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::reallocate(size_type new_cap)
	{
		new_cap = ge_near_pow2(new_cap + 1); //+1 for '\0'

		auto new_buffer = data_allocator::allocate(new_cap);

		//避免构造函数调用时buffer_为空
		if (buffer_ != nullptr) {
			memmove(new_buffer, buffer_, size_);
			free();
		}


		buffer_ = new_buffer;
		cap_ = new_cap - 1;
		buffer_[cap_] = 0;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type
		basic_string<CharT, Alloc>::ge_near_pow2(size_type n)
	{
		//最接近n的大于等于n的2的倍数
		if ((n & (n - 1)) == 0)
			return n;
		while ((n & (n - 1)) != 0) {
			n = n & (n - 1);
		}
		return n << 1;
	}

	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::free() {
		if (buffer_ != nullptr) {
			if (buffer_ != init_buffer_) {
				data_allocator::deallocate(buffer_, cap_ + 1);
			}
		}
		buffer_ = nullptr;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string()
		:size_(0),
		cap_(kInitCapacity),
		buffer_(init_buffer_)
	{
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(size_type count, CharT ch)
		:size_(count),
		cap_(0),
		buffer_(init_buffer_)
	{
		chooseBuffer();
		memset(buffer_, ch, count);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(const basic_string& str)
		:size_(str.size_),
		cap_(0),
		buffer_(nullptr)
	{
		chooseBuffer();
		memmove(buffer_, str.buffer_, size_);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(const basic_string& str, size_type pos, size_type count)
		:size_(),
		cap_(0),
		buffer_(nullptr)
	{
		ASSERT_EXPR(pos < str.size());
		count = mystl::min(count, str.size_ - pos);
		size_ = count;
		chooseBuffer();

		//一一复制
		auto src = str.cbegin() + pos;
		auto dst = buffer_;
		for (; count != 0; --count, ++dst, ++src)
			*dst = *src;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(const CharT* cstr) :basic_string(cstr, strlen(cstr))
	{
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(const CharT* cstr, size_type len)
		: size_(len),
		cap_(0),
		buffer_(nullptr)
	{
		ASSERT_EXPR(strlen(cstr) >= len);
		chooseBuffer();
		memmove(buffer_, cstr, size_);
	}

	template<typename CharT, typename Alloc>
	template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline basic_string<CharT, Alloc>::basic_string(InputIt first, InputIt last)
		:size_(last - first),
		cap_(0),
		buffer_(nullptr)
	{
		chooseBuffer();
		memmove(buffer_, first, size_);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(basic_string&& str) noexcept
		:size_(str.size()),
		cap_(str.cap_)
	{
		if (str.buffer_ == str.init_buffer_) {   //要移动的str没有分配堆内存
			buffer_ = init_buffer_;
			memmove(buffer_, str.buffer_, size_);
		}
		else {
			buffer_ = str.buffer_;
		}
		str.size_ = 0;
		str.cap_ = 0;
		str.buffer_ = nullptr;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::basic_string(std::initializer_list<CharT> ilist)
		:basic_string(ilist.begin(),ilist.end())
	{
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>::~basic_string() {
		free();
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator=(const basic_string& rhs)
	{
		if (this != &rhs)
			return operator=(rhs.c_str());
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator=(const_pointer cstr)
	{
		size_ = strlen(cstr);
		if (size_ <= capacity()) {  //容量足够
			memmove(buffer_, cstr, size_);
		}
		else {
			free();
			chooseBuffer();
			memmove(buffer_, cstr, size_);
		}
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator=(value_type ch)
	{
		size_ = 1;
		free();
		chooseBuffer();
		buffer_[0] = ch;
		return *this;

	}

	template<typename CharT, typename Alloc>
	basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator=(basic_string&& rhs) noexcept
	{
		if (this != &rhs) {
			free();
			size_ = rhs.size_;
			cap_ = rhs.cap_;
			if (rhs.buffer_ == rhs.init_buffer_) {   //要移动的str没有分配堆内存
				buffer_ = init_buffer_;
				memmove(buffer_, rhs.buffer_, size_);
			}
			else {
				buffer_ = rhs.buffer_;
			}
			rhs.size_ = 0;
			rhs.cap_ = 0;
			rhs.buffer_ = nullptr;
		}
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::assign(const basic_string& str)
	{
		*this = str;
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::assign(const_pointer cstr)
	{
		*this = cstr;
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::assign(iterator first, iterator last)
	{
		size_type count = last - first;

		//自赋值，一定不会扩容
		if (cap_ < count) {
			reallocate(count);
		}
		memmove(begin(), first, count);
		size_ = count;

		return *this;
	}

	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::reserve(size_type new_cap)
	{
		if (cap_ < new_cap) {
			reallocate(new_cap);
		}
	}

	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::shrink_to_fit()
	{
		//不是初始buffer才执行
		if (buffer_ != init_buffer_) {
			if (size_ != cap_) {
				size_type new_cap = ge_near_pow2(size_ + 1);
				if (cap_ != new_cap) { //cap_不同
					auto new_buffer = data_allocator::allocate(new_cap);
					memmove(new_buffer, buffer_, size_);
					data_allocator::deallocate(buffer_, cap_ + 1);
					buffer_ = new_buffer;
					cap_ = new_cap;
				}
			}
		}
	}

	//在位置 index 插入 count 个字符 ch 的副本
	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>&
		basic_string<CharT, Alloc>::insert(size_type index, size_type count, CharT ch)
	{
		ASSERT_EXPR(index <= size_);

		if (cap_ - size_ < count) {
			reallocate(size_ + count);
		}
		//先将元素添加到尾部，然后旋转到指定位置
		memset(end(), ch, count);
		size_ += count;
		iterator rotate_first = buffer_ + index;
		mystl::rotate(rotate_first, end() - count, end());

		return *this;

	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::insert(size_type index, const CharT* s)
	{
		insert(buffer_ + index, s, s + strlen(s));
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::insert(size_type index, const CharT* s, size_type count)
	{
		insert(buffer_ + index, s, s + count);
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>&
		basic_string<CharT, Alloc>::insert(size_type index, const basic_string& str)
	{
		insert(buffer_ + index, str.begin(), str.end());
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::insert(size_type index, const basic_string& str,
			size_type index_str, size_type count)
	{
		insert(buffer_ + index, str.begin() + index_str, 
			str.begin() + index_str + mystl::min(count,str.size_ - index_str));
		return *this;
	}


	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::iterator
		basic_string<CharT, Alloc>::insert(const_iterator pos, CharT ch)
	{
		return insert(pos, 1, ch);
	}

	//在 pos 所指向的元素（若存在）前插入 count 个字符 ch 的副本
	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::iterator
		basic_string<CharT, Alloc>::insert(const_iterator pos, size_type count, CharT ch)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end());

		//避免扩容引发迭代器失效，提前记录下标
		size_type pos_index = pos - buffer_;
		insert(pos_index, count, ch);
		return buffer_ + pos_index;
	}


	//在 pos 所指向的元素（若存在）前插入来自范围 [first, last) 的元素
	template<typename CharT, typename Alloc>
	template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	typename basic_string<CharT, Alloc>::iterator
		basic_string<CharT, Alloc>::insert(const_iterator pos, InputIt first, InputIt last)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end());

		size_type count = last - first;

		if (cap_ - size_ < count) {
			//避免扩容引发迭代器失效，提前记录下标
			size_type pos_index = pos - buffer_;
			//从自己插入
			if (first >= begin() && first <= end()) {
				size_type first_index = end() - first;
				reallocate(size_ + count);
				first = buffer_ + first_index;
			}
			else {
				reallocate(size_ + count);
			}
			pos = buffer_ + pos_index;
		}

		//先把要插入的元素添加到尾部，然后旋转到指定位置
		memmove(end(), first, count);
		size_ += count;
		iterator rotate_first = const_cast<iterator>(pos);
		mystl::rotate(rotate_first, end() - count, end()); //end() - count 要插入的元素起始位置

		return rotate_first;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::iterator 
		basic_string<CharT, Alloc>::insert(const_iterator pos, std::initializer_list<CharT> ilist)
	{
		return insert(pos, ilist.begin(), ilist.end());
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::erase(size_type index, size_type count)
	{
		erase(buffer_ + index, buffer_ + index + mystl::min(count, size_ - index));
		return *this;
	}


	//移除位于 pos 的字符
	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::iterator
		basic_string<CharT, Alloc>::erase(const_iterator pos)
	{
		ASSERT_EXPR(pos >= begin() && pos < end());
		iterator dst = const_cast<iterator>(pos);
		const_iterator src = pos + 1;
		memmove(dst, src, end() - pos - 1);
		size_--;
		buffer_[size_] = 0;
		return dst;
	}

	//移除范围 [first, last) 中的元素
	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::iterator
		basic_string<CharT, Alloc>::erase(const_iterator first, const_iterator last)
	{
		ASSERT_EXPR(first >= begin() && last <= end() && first <= last);
		size_type count = last - first;
		iterator dst = const_cast<iterator>(first);
		auto move_count = end() - last;

		memmove(dst, last, move_count);
		size_ -= count;
		return dst;
	}


	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(size_type count, CharT ch)
	{
		if (cap_ - size_ < count) {
			reallocate(size_ + count);
		}
		CharT* dst = buffer_ + size_;
		memset(dst, ch, count);
		size_ += count;
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(const basic_string& str)
	{
		return append(str, 0, str.size_);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(const basic_string& str, size_type pos, size_type count)
	{
		ASSERT_EXPR(pos < str.size());
		count = mystl::min(count, str.size_ - pos);
		if (cap_ - size_ < count) {
			reallocate(size_ + count);
		}
		auto dst = buffer_ + size_;
		memmove(dst, str.buffer_ + pos, count);
		size_ += count;
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(const CharT* cstr, size_type count)
	{
		return append(cstr, cstr + mystl::min(strlen(cstr), count)); 
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(const CharT* cstr)
	{
		return append(cstr, strlen(cstr));
	}

	template<typename CharT, typename Alloc>
	template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(InputIt first, InputIt last)
	{
		size_type count = last - first;
		if (cap_ - size_ < count) {
			//从自己添加
			if (first >= begin() && first <= end()) {
				size_type first_index = first - begin();
				reallocate(size_ + count);
				first = buffer_ + first_index;
			}
			else {
				reallocate(size_ + count);
			}
		}
		auto dst = end();
		memmove(dst, first, count);
		size_ += count;
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::append(std::initializer_list<CharT> ilist)
	{
		return append(ilist.begin(), ilist.end());
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator+=(const basic_string& str)
	{
		return append(str);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator+=(CharT ch)
	{
		return append(1, ch);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator+=(const CharT* cstr)
	{
		return append(cstr);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::operator+=(std::initializer_list<CharT> ilist)
	{
		return append(ilist.begin, ilist.end());
	}


	template<typename CharT, typename Alloc>
	inline int basic_string<CharT, Alloc>::compare(size_type pos1, size_type count1, const basic_string& str) const
	{
		return compare(pos1, count1, str.c_str(), str.size());
	}

	template<typename CharT, typename Alloc>
	inline int basic_string<CharT, Alloc>::compare(size_type pos1, size_type count1,
		const basic_string& str, size_type pos2, size_type count2) const
	{
		ASSERT_EXPR(pos2 < str.size_);
		return compare(pos1, count1, str.c_str() + pos2, mystl::min(str.size_ - pos2, count2));
	}

	template<typename CharT, typename Alloc>
	inline int basic_string<CharT, Alloc>::compare(size_type pos1, size_type count1, const CharT* s) const
	{
		return compare(pos1, count1, s, strlen(s));
	}

	template<typename CharT, typename Alloc>
	int basic_string<CharT, Alloc>::compare(size_type pos1, size_type count1, const CharT* s, size_type count2) const
	{
		ASSERT_EXPR(pos1 < size_);
		auto first1 = buffer_ + pos1;
		auto last1 = first1 + mystl::min(size_ - pos1, count1);
		auto first2 = s;
		auto last2 = first2 + mystl::min(strlen(s), count2);
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
			if (*first1 < *first2) return -1;
			if (*first2 < *first1) return 1;
		}
		if (first1 == last1 && first2 == last2) {
			return 0;
		}
		else if (first1 == last1 && first2 != last2) {
			return -1;
		}
		else {
			return 1;
		}
	}


	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(size_type pos, size_type count,const basic_string& str)
	{
		return replace(buffer_ + pos, buffer_ + pos + count, str.begin(), str.end());
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(const_iterator first, const_iterator last,const basic_string& str)
	{
		return replace(first, last, str.begin(), str.end());
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(size_type pos, size_type count,
			const basic_string& str,size_type pos2, size_type count2)
	{
		return replace(buffer_ + pos, buffer_ + pos + count,
			str.c_str() + pos2, str.c_str() + pos2 + mystl::min(str.size_ - pos2, count2));
	}

	template<typename CharT, typename Alloc>
	template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(const_iterator first, const_iterator last,
		InputIt first2, InputIt last2)
	{
		ASSERT_EXPR(first >= buffer_ && first <= buffer_ + size_ && first <= last && first2 <= last2);
		auto first1 = const_cast<iterator>(first);
		auto last1 = const_cast<iterator>(last);
		size_type count1 = last1 - first1;
		size_type count2 = last2 - first2;
		if (count1 < count2) {
			size_type diff = count2 - count1;
			if ((cap_ - size_) < diff) { //剩余空间不足
				//扩容会使迭代器失效，记录下标
				size_type first1_index = first1 - buffer_;
				size_type last1_index = last1 - buffer_;
				if (first2 >= begin() && first2 <= end()) { //first2指向自己
					size_type first2_index = first2 - buffer_;
					reallocate(size_ + diff);
					first2 = buffer_ + first2_index;
				}
				else {
					reallocate(size_ + diff);
				}
				first1 = buffer_ + first1_index;
				last1 = buffer_ + last1_index;
			}
			memmove(last1 + diff, last1, end() - last1);//空出count2个空间
			memmove(first1, first2, count2);
			size_ += (count2 - count1);
		}
		else {
			memmove(first1, first2, count2);
			memmove(first1 + count2, last1, end() - last1);//填补空出的字符
			size_ -= (count1 - count2);
		}
		return *this;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(size_type pos, size_type count,const CharT* cstr, size_type count2)
	{
		return replace(buffer_ + pos, buffer_ + pos + count, cstr, cstr + mystl::min(strlen(cstr), count2));
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(const_iterator first, const_iterator last,
		const CharT* cstr, size_type count2)
	{
		return replace(first, last, cstr, cstr + mystl::min(strlen(cstr), count2));
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(size_type pos, size_type count,const CharT* cstr)
	{
		return replace(buffer_ + pos, buffer_ + pos + count, cstr, cstr + strlen(cstr));
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(const_iterator first, const_iterator last,const CharT* cstr)
	{
		return replace(first, last, cstr, cstr + strlen(cstr));
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& 
		basic_string<CharT, Alloc>::replace(size_type pos, size_type count,size_type count2, CharT ch)
	{
		ASSERT_EXPR(pos < size_&& pos + count < size_);
		basic_string tmp(count2, ch);
		return replace(pos, count, tmp);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::replace(const_iterator first, const_iterator last,
		size_type count2, CharT ch)
	{
		ASSERT_EXPR(first >= buffer_ && first <= buffer_ + size_ && first < last);
		basic_string tmp(count2, ch);
		return replace(first, last, tmp);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::replace(const_iterator first, const_iterator last,
		std::initializer_list<CharT> ilist)
	{
		return replace(first, last, ilist.begin(), ilist.end());
	}
	
	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc> 
		basic_string<CharT, Alloc>::substr(size_type pos, size_type count) const
	{
		ASSERT_EXPR(pos < size_);
		return basic_string(buffer_ + pos, buffer_ + pos + mystl::min(count, size_ - pos));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::copy(CharT* dest, size_type count, size_type pos) const
	{
		ASSERT_EXPR(pos < size_);
		memmove(dest, buffer_ + pos, mystl::min(count, size_ - pos));
	}

	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::resize(size_type count)
	{
		resize(count, CharT());
	}

	template<typename CharT, typename Alloc>
	inline void basic_string<CharT, Alloc>::resize(size_type count, CharT ch)
	{
		if (size_ < count) {
			append(count - size_, ch);
		}
		else if(size_ > count){
			erase(count, size_ - count);
		}
	}

	template<typename CharT, typename Alloc>
	void basic_string<CharT, Alloc>::swap(basic_string& rhs) noexcept
	{
		if (this != &rhs) {
			if (rhs.buffer_ == rhs.init_buffer_ && buffer_ == init_buffer_) {
				//复制内容
				value_type tmp[kInitBufferSize];
				memcpy(tmp, init_buffer_, kInitBufferSize);
				memcpy(init_buffer_, rhs.init_buffer_, kInitBufferSize);
				memcpy(rhs.init_buffer_, tmp, kInitBufferSize);
			}
			else if (rhs.buffer_ == rhs.init_buffer_) {
				memcpy(init_buffer_, rhs.init_buffer_, kInitBufferSize);
				rhs.buffer_ = buffer_;
				buffer_ = init_buffer_;
			}
			else if (buffer_ == init_buffer_) {
				memcpy(rhs.init_buffer_, init_buffer_, kInitBufferSize);
				buffer_ = rhs.buffer_;
				rhs.buffer_ = init_buffer_;
			}
			else { //堆内存直接交换
				mystl::swap(buffer_, rhs.buffer_);
			}
			mystl::swap(size_, rhs.size_);
			mystl::swap(cap_, rhs.cap_);
		}
	}
	
	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find(const basic_string& str, size_type pos) const noexcept
	{
		return find(str.c_str(), pos, str.size_);
	}

	template<typename CharT, typename Alloc>
	typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find(const CharT* s, size_type pos, size_type count) const
	{
		if (count == 0) {
			return pos;
		}	
		if (size_ - pos < count) {//长度不足
			return npos;
		}
		size_type limit = size_ - count;
		for (; pos <= limit; ++pos) {
			//首字符相同才继续
			if (*(buffer_ + pos) == *s) {
				size_type i = 1;
				for (; i < count; ++i) {
					if (*(buffer_ + pos + i) != *(s + i)) {
						break;
					}
				}
				if (i == count) { //所有字符都相同
					return pos;
				}
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find(const CharT* s, size_type pos) const
	{
		return find(s,pos,strlen(s));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find(CharT ch, size_type pos) const noexcept
	{
		for (; pos < size_; ++pos) {
			if (*(buffer_ + pos) == ch) {
				return pos;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::rfind(const basic_string& str, size_type pos) const noexcept
	{
		return rfind(str.c_str(), pos, str.size_);
	}

	//从尾部反向查找
	template<typename CharT, typename Alloc>
	typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::rfind(const CharT* s, size_type pos, size_type count) const
	{
		if (count == 0) {
			return pos;
		}	
		if (pos >= size_) {
			pos = size_ - 1;
		}
		if (pos < count - 1) {//长度不足
			return npos;
		}	
		int limit = static_cast<int>(count - 1);
		for (int i = static_cast<int>(pos); i >= limit; --i)
		{
			if (*(buffer_ + i) == *(s + count - 1)) //尾字符相同
			{
				size_type j = 1;
				for (; j < count; ++j)
				{
					if (*(buffer_ + i - j) != *(s + count - j - 1))
						break;
				}
				if (j == count)
					return i - count + 1;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::rfind(const CharT* s, size_type pos) const
	{
		return rfind(s,pos,strlen(s));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::rfind(CharT ch, size_type pos) const noexcept
	{
		if (pos >= size_) {
			pos = size_ - 1;
		}
		for (int i = static_cast<int>(pos); i >= 0; --i) {
			if (*(buffer_ + i) == ch) {
				return i;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_of(const basic_string& str, size_type pos) const noexcept
	{
		return find_first_of(str.c_str(), pos, str.size_);
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_of(const CharT* s, size_type pos, size_type count) const
	{
		for (; pos < size_; ++pos) {
			CharT ch = *(buffer_ + pos);
			for(size_type i = 0;i < count;++i){
				if (ch == *(s + i)) {
					return pos;
				}
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_of(const CharT* s, size_type pos) const
	{
		return find_first_of(s, pos, strlen(s));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_of(CharT ch, size_type pos) const noexcept
	{
		for (; pos < size_; ++pos) {
			if (*(buffer_ + pos) == ch) {
				return pos;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_not_of(const basic_string& str, size_type pos) const noexcept
	{
		return find_first_not_of(str.c_str(), pos, str.size_);
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_not_of(const CharT* s, size_type pos, size_type count) const
	{
		for (; pos < size_; ++pos) {
			CharT ch = *(buffer_ + pos);
			bool flag = false;
			for (size_type i = 0; i < count; ++i) {
				if (ch == *(s + i)) {
					flag = true;
					break;
				}
			}
			if (!flag) { //所有字符都与ch不相等
				return pos;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_not_of(const CharT* s, size_type pos) const
	{
		return find_first_not_of(s, pos, strlen(s));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_first_not_of(CharT ch, size_type pos) const noexcept
	{
		for (; pos < size_; ++pos) {
			if (*(buffer_ + pos) != ch) {
				return pos;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_of(const basic_string& str, size_type pos) const noexcept
	{
		return find_last_of(str.c_str(), pos, str.size_);
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_of(const CharT* s, size_type pos, size_type count) const
	{
		if (pos >= size_) {
			pos = size_ - 1;
		}
		for (int i = static_cast<int>(pos); i >= 0; --i) {
			CharT ch = *(buffer_ + i);
			for (size_type j = 0; j < count; ++j) {
				if (ch == *(s + j)) {
					return i;
				}
			}
		}
		return npos ;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_of(const CharT* s, size_type pos) const
	{
		return find_last_of(s, pos, strlen(s));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_of(CharT ch, size_type pos) const noexcept
	{
		if (pos >= size_) {
			pos = size_ - 1;
		}
		for (int i = static_cast<int>(pos); i >= 0; --i)
		{
			if (*(buffer_ + i) == ch)
				return i;
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_not_of(const basic_string& str, size_type pos) const noexcept
	{
		return find_last_not_of(str.c_str(), pos, str.size_);
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_not_of(const CharT* s, size_type pos, size_type count) const
	{
		if (pos >= size_) {
			pos = size_ - 1;
		}
		for (int i = static_cast<int>(pos); i >= 0; --i) {
			CharT ch = *(buffer_ + i);
			bool flag = false;
			for (size_type j = 0; j < count; ++j) {
				if (ch == *(s + j)) {
					flag = true;
					break;
				}
			}
			if (!flag) { //所有字符都与ch不相等
				return i;
			}
		}
		return npos;
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_not_of(const CharT* s, size_type pos) const
	{
		return find_last_not_of(s, pos, strlen(s));
	}

	template<typename CharT, typename Alloc>
	inline typename basic_string<CharT, Alloc>::size_type 
		basic_string<CharT, Alloc>::find_last_not_of(CharT ch, size_type pos) const noexcept
	{
		if (pos >= size_) {
			pos = size_ - 1;
		}
		for (int i = static_cast<int>(pos); i >= 0; --i)
		{
			if (*(buffer_ + i) != ch)
				return i;
		}
		return npos;
	}



	using string = mystl::basic_string<char>; //string

	//非成员函数
	template<typename CharT, typename Alloc>
	inline void swap(basic_string<CharT, Alloc>& lhs, basic_string<CharT, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc> operator+(const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs)
	{
		basic_string<CharT, Alloc> str(lhs);
		str += rhs;
		return str;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc> operator+(const basic_string<CharT, Alloc>& lhs, const char* rhs)
	{
		basic_string<CharT, Alloc> str(lhs);
		str += rhs;
		return str;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc> operator+(const char* lhs, const basic_string<CharT, Alloc>& rhs)
	{
		basic_string<CharT, Alloc> str(lhs);
		str += rhs;
		return str;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc> operator+(const basic_string<CharT, Alloc>& lhs, char rhs)
	{
		basic_string<CharT, Alloc> str(lhs);
		str += rhs;
		return str;
	}

	template<typename CharT, typename Alloc>
	inline basic_string<CharT, Alloc> operator+(char lhs, const basic_string<CharT, Alloc>& rhs)
	{
		basic_string<CharT, Alloc> str(&lhs);
		str += rhs;
		return str;
	}


	template<typename CharT, typename Alloc>
	bool operator==(const basic_string<CharT, Alloc>& lhs,
		const basic_string<CharT, Alloc>& rhs)
	{
		return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
	}

	template<typename CharT, typename Alloc>
	bool operator!=(const basic_string<CharT, Alloc>& lhs,
		const basic_string<CharT, Alloc>& rhs)
	{
		return lhs.size() != rhs.size() || lhs.compare(rhs) != 0;
	}

	template<typename CharT, typename Alloc>
	bool operator<(const basic_string<CharT, Alloc>& lhs,
		const basic_string<CharT, Alloc>& rhs)
	{
		return lhs.compare(rhs) < 0;
	}

	template<typename CharT, typename Alloc>
	bool operator<=(const basic_string<CharT, Alloc>& lhs,
		const basic_string<CharT, Alloc>& rhs)
	{
		return lhs.compare(rhs) <= 0;
	}

	template<typename CharT, typename Alloc>
	bool operator>(const basic_string<CharT, Alloc>& lhs,
		const basic_string<CharT, Alloc>& rhs)
	{
		return lhs.compare(rhs) > 0;
	}

	template<typename CharT, typename Alloc>
	bool operator>=(const basic_string<CharT, Alloc>& lhs,
		const basic_string<CharT, Alloc>& rhs)
	{
		return lhs.compare(rhs) >= 0;
	}

	//operator<< , operator>>
	template<typename CharT, typename Alloc>
	std::ostream& operator<<(std::ostream& os,const mystl::basic_string<CharT, Alloc>& str) {
		os << str.data();
		return os;
	}

	template<typename CharT,typename Alloc>
	std::istream& operator>>(std::istream& is, const mystl::basic_string<CharT, Alloc>& str) {
		CharT* buf = new CharT[4096];
		is >> buf;
		basic_string<CharT, Alloc> tmp(buf);
		str = std::move(tmp);
		delete[]buf;
		return is;
	}

	//https://www.zhihu.com/question/422840340
	template<>
	struct hash<mystl::string> {
		int operator()(const mystl::string& str) const {
			int h = 0;
			int n = static_cast<int>(str.size());
			for (int i = 0; i < n; ++i) {
				h = 31 * h + str[i];
			}
			return h;
		}
	};
	



} //end of namespace mystl

#endif //!_MY_basic_string_H_