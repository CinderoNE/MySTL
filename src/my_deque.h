#pragma once
#ifndef _MY_DEQUE_H_
#define _MY_DEQUE_H_

#include"my_algorithm.h"
#include"my_allocator.h"
#include"my_iterator.h"
#include"my_uninitialized.h"

namespace mystl {

#ifndef DEQUE_BUF_BYTES
#define DEQUE_BUF_BYTES 512  //单个缓冲区的字节数
#endif

#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8 //初始缓冲区数量
#endif

	//单个缓冲区的元素数量
	//单个元素大小大于512个字节则一个缓冲区存一个元素
	template<typename T>
	struct deque_buf_size {
		static constexpr size_t size = sizeof(T) < DEQUE_BUF_BYTES ? DEQUE_BUF_BYTES / sizeof(T) : 1;
	};

	template<typename T>
	struct deque_const_iterator : public mystl::iterator<random_access_iterator_tag, T> {

		using self = deque_const_iterator;

		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

		using value_pointer = value_type*;
		using map_pointer = value_type**;

		value_pointer curr;    // 指向所在缓冲区的当前元素
		value_pointer first;  // 指向所在缓冲区的头部
		value_pointer last;   // 指向所在缓冲区的尾部
		map_pointer   node;   // 所在缓冲区

		static const size_type buffer_size = deque_buf_size<value_type>::size; //单个缓冲区的元素数量

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *node;
			last = first + buffer_size;
		}

		deque_const_iterator() noexcept
			:curr(nullptr), first(nullptr), last(nullptr), node(nullptr)
		{
		}

		deque_const_iterator(value_pointer v_p, map_pointer m_p)
			:curr(v_p), first(*m_p), last(first + buffer_size), node(m_p)
		{
		}

		deque_const_iterator(const self& rhs)
			:curr(rhs.curr), first(rhs.first), last(rhs.last), node(rhs.node)
		{
		}

		deque_const_iterator(self&& rhs) noexcept
			:curr(rhs.curr), first(rhs.first), last(rhs.last), node(rhs.node)
		{
			rhs.curr = nullptr;
			rhs.first = nullptr;
			rhs.last = nullptr;
			rhs.node = nullptr;
		}


		self& operator=(const self& rhs) {
			if (this != &rhs)
			{
				curr = rhs.curr;
				first = rhs.first;
				last = rhs.last;
				node = rhs.node;
			}
			return *this;
		}

		reference operator*() const {
			return *curr;
		}

		pointer operator->() const {
			return curr;
		}

		self& operator++() {
			++curr;
			if (curr == last) {
				set_node(node + 1);
				curr = first;
			}
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--() {
			if (curr == first) {
				set_node(node - 1);
				curr = last;
			}
			--curr;
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}


		self& operator+=(difference_type off) {
			difference_type offset_to_first = curr - first;
			difference_type offset = off + offset_to_first;
			//仍在当前node中
			if (offset >= 0 && static_cast<size_type>(offset) < buffer_size) {
				curr += off;
			}
			else {
				//offset > 0往前移，< 0往后移
				difference_type node_offset = offset > 0 ?
					static_cast<size_type>(offset) / buffer_size
					: -static_cast<difference_type>((-offset + buffer_size) / buffer_size);
				set_node(node + node_offset);
				curr = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
			}
			return *this;
		}

		self operator+(difference_type off) const {
			self tmp = *this;
			return tmp += off;
		}

		self& operator-=(difference_type off) {
			return *this += -off;
		}

		self operator-(difference_type off) const {
			self tmp = *this;
			return tmp -= off;
		}

		difference_type operator-(const self& rhs) const {
			return buffer_size * (node - rhs.node) +
				(curr - first) - (rhs.curr - rhs.first);
		}

		reference operator[](difference_type off) const {
			return *(*this + off);
		}


		//比较操作
		bool operator==(const self& rhs) const
		{
			return curr == rhs.curr;
		}

		bool operator!=(const self& rhs) const
		{
			return !(*this == rhs);
		}
		
		bool operator<(const self& rhs) const
		{
			return node == rhs.node ? (curr < rhs.curr) : (node < rhs.node);
		}

		bool operator<=(const self& rhs) const
		{
			return !(rhs < *this);
		}

		bool operator>(const self& rhs) const
		{
			return rhs < *this;
		}

		bool operator>=(const self& rhs) const
		{
			return !(*this < rhs);
		}
	};

	template<typename T>
	struct deque_iterator : public deque_const_iterator<T> {
		using self = deque_iterator;

		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

		using value_pointer = value_type*;
		using map_pointer = value_type**;


		deque_iterator() noexcept
			:deque_const_iterator<T>()
		{
		}

		deque_iterator(value_pointer v_p, map_pointer m_p)
			:deque_const_iterator<T>(v_p, m_p)
		{
		}

		deque_iterator(const self& rhs)
			:deque_const_iterator<T>(rhs)
		{
		}

		deque_iterator(self&& rhs) noexcept
			:deque_const_iterator<T>(std::move(rhs))
		{
		}

		using deque_const_iterator<T>::curr;
		using deque_const_iterator<T>::first;
		using deque_const_iterator<T>::last;
		using deque_const_iterator<T>::node;
		using deque_const_iterator<T>::buffer_size;

		self& operator=(const self& rhs) {
			if (this != &rhs)
			{
				curr = rhs.curr;
				first = rhs.first;
				last = rhs.last;
				node = rhs.node;
			}
			return *this;
		}

		reference operator*() const {
			return *curr;
		}

		pointer operator->() const {
			return curr;
		}

		using deque_const_iterator<T>::set_node;

		self& operator++() {
			++curr;
			if (curr == last) {
				set_node(node + 1);
				curr = first;
			}
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--() {
			if (curr == first) {
				set_node(node - 1);
				curr = last;
			}
			--curr;
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}

		self& operator+=(difference_type off) {
			difference_type offset_to_first = curr - first;
			difference_type offset = off + offset_to_first;
			//仍在当前node中
			if (offset >= 0 && static_cast<size_type>(offset) < buffer_size) {
				curr += off;
			}
			else {
				//offset > 0往前移，< 0往后移
				difference_type node_offset = offset > 0 ?
					static_cast<size_type>(offset) / buffer_size
					: -static_cast<difference_type>((-offset + buffer_size) / buffer_size);
				set_node(node + node_offset);
				curr = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
			}
			return *this;
		}

		self operator+(difference_type off) const {
			self tmp = *this;
			return tmp += off;
		}

		self& operator-=(difference_type off) {
			return *this += -off;
		}

		self operator-(difference_type off) const {
			self tmp = *this;
			return tmp -= off;
		}

		difference_type operator-(const self& rhs) const {
			return buffer_size * (node - rhs.node) +
				(curr - first) - (rhs.curr - rhs.first);
		}

		reference operator[](difference_type off) const {
			return *(*this + off);
		}
	};

	/*template<typename Mydeque>
	bool operator==(const deque_const_iterator<Mydeque>& lhs, const deque_const_iterator<Mydeque>& rhs)
	{
		return lhs.curr == rhs.curr;
	}

	template<typename Mydeque>
	bool operator!=(const deque_const_iterator<Mydeque>& lhs, const deque_const_iterator<Mydeque>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Mydeque>
	bool operator<(const deque_const_iterator<Mydeque>& lhs, const deque_const_iterator<Mydeque>& rhs)
	{
		return lhs.node == rhs.node ? (lhs.curr < rhs.curr) : (lhs.node < rhs.node);
	}

	template<typename Mydeque>
	bool operator<=(const deque_const_iterator<Mydeque>& lhs, const deque_const_iterator<Mydeque>& rhs)
	{
		return !(rhs < lhs);
	}

	template<typename Mydeque>
	bool operator>(const deque_const_iterator<Mydeque>& lhs, const deque_const_iterator<Mydeque>& rhs)
	{
		return rhs < lhs;
	}

	template<typename Mydeque>
	bool operator>=(const deque_const_iterator<Mydeque>& lhs, const deque_const_iterator<Mydeque>& rhs)
	{
		return !(lhs < rhs);
	}*/

	template<typename T, typename Alloc = allocator<T>>
	class deque {
	public:
		using value_type = T;
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = deque_iterator<T>;
		using const_iterator = deque_const_iterator<T>;
		using reverse_iterator = mystl::reverse_iterator<iterator>;
		using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

		using map_allocator = allocator<T*>;
		using map_pointer = pointer*;
		using const_map_pointer = const_pointer*;

		using data_allocator = Alloc;
		allocator_type get_allocator() { return data_allocator(); }

		static const size_type buffer_size = deque_buf_size<T>::size;
	private:
		iterator       begin_;      // 指向第一个节点
		iterator       end_;		// 指向尾后节点
		map_pointer    map_;        // 指向中控区 ，其中的每个元素都是一个指针，指向一个缓冲区
		size_type      map_size_;   // 中控区内指针的数目，也就是缓冲区的数量
	private:
		map_pointer allocate_map(size_type map_size);
		void map_initialize(size_type count);//初始化中控区
		void fill_initialize(size_type count, const value_type& value);
		template<typename InputIt>
		void range_initialize(InputIt first, InputIt last);
		template<typename InputIt>
		void range_assign(InputIt first, InputIt last);
		//front:
		//		true:确保前半部分有足够空间
		//		false:确保后半部分有足够空间
		void ensure_capacity(size_type count, bool front);
		//扩容中控区
		//front:
		//		true:给前半部分空出need_buffer个缓冲区
		//      false:给后半部分空出need_buffer个缓冲区
		void reallocate_map(size_type need_buffer, bool front);
		void free();
	public:
		deque();
		explicit deque(size_type count);
		deque(size_type count, const T& value);
		template< typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0 >
		deque(InputIt first, InputIt last);
		deque(const deque& other);
		deque(deque&& other) noexcept;
		deque(std::initializer_list<T> init);
		~deque();

		deque& operator=(const deque& other);
		deque& operator=(deque&& other) noexcept;
		deque& operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T& value);
		template< typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0 >
		void assign(InputIt first, InputIt last);
		void assign(std::initializer_list<T> ilist);
	public:
		//元素访问
		reference       at(size_type pos) { return (*this)[pos]; }
		const_reference at(size_type pos) const { return (*this)[pos]; }

		reference operator[](size_type pos)
		{
			ASSERT_EXPR(pos < size());
			return begin_[pos];
		}
		const_reference operator[](size_type pos) const
		{
			ASSERT_EXPR(pos < size());
			return begin_[pos];
		}

		reference front()
		{
			ASSERT_EXPR(!empty());
			return *begin_;
		}
		const_reference front() const
		{
			ASSERT_EXPR(!empty());
			return *begin_;
		}

		reference back()
		{
			ASSERT_EXPR(!empty());
			return *(end_ - 1);
		}
		const_reference back() const
		{
			ASSERT_EXPR(!empty());
			return *(end_ - 1);
		}

		//迭代器
		iterator begin() noexcept { return begin_; }
		const_iterator begin() const noexcept { return begin_; }
		const_iterator cbegin() const noexcept { return begin_; }

		iterator end() noexcept { return end_; }
		const_iterator end() const noexcept { return end_; }
		const_iterator cend() const noexcept { return end_; }

		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
		const_reverse_iterator crbegin() const noexcept { return reverse_iterator(end()); }

		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator crend() const noexcept { return reverse_iterator(begin()); }

	public:
		//容量
		bool empty() const noexcept { return begin_ == end_; }
		size_type size() const noexcept { return end_ - begin_; }
		void shrink_to_fit();
	private:
		template< class... Args >
		iterator __insert(const_iterator pos, Args&&...args);
		iterator fill_insert(const_iterator pos, size_type count, const T& value);
		template< typename InputIt>
		iterator range_insert(const_iterator pos, InputIt first, InputIt last);
	public:
		//修改器
		void clear() noexcept;

		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);
		template< typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0 >
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

		void pop_back();

		void push_front(const T& value);
		void push_front(T&& value);

		template< class... Args >
		void emplace_front(Args&&... args);

		void pop_front();

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(deque& other) noexcept;

	};

	template<typename T, typename Alloc>
	inline typename deque<T, Alloc>::map_pointer
		deque<T, Alloc>::allocate_map(size_type map_size)
	{
		map_pointer map_p = map_allocator::allocate(map_size);
		for (auto curr = map_p; curr < map_p + map_size; ++curr) {
			*curr = nullptr;
		}
		return map_p;
	}

	template<typename T, typename Alloc>
	void deque<T, Alloc>::map_initialize(size_type count)
	{
		size_type buffer_count = count / buffer_size + 1;
		//+2是为了前后方便扩展
		map_size_ = mystl::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), buffer_count + 2);
		map_ = allocate_map(map_size_);

		//使指向的缓冲区位于中央，方便前后扩展
		map_pointer map_start = map_ + (map_size_ - buffer_count) / 2;
		map_pointer map_end = map_start + buffer_count - 1;

		for (map_pointer curr = map_start; curr <= map_end; ++curr) {
			*curr = data_allocator::allocate(buffer_size);
		}

		begin_.set_node(map_start);
		begin_.curr = begin_.first;
		end_.set_node(map_end);
		end_.curr = end_.first + (count % buffer_size);
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::fill_initialize(size_type count, const value_type& value)
	{
		map_initialize(count);
		if (count > 0) {
			for (auto curr_node = begin_.node; curr_node < end_.node; ++curr_node) {
				mystl::uninitialized_fill_n(*curr_node, buffer_size, value);
			}
			mystl::uninitialized_fill(end_.first, end_.curr, value);
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIt>
	inline void deque<T, Alloc>::range_initialize(InputIt first, InputIt last)
	{
		size_type count = mystl::distance(first, last);
		map_initialize(count);
		mystl::uninitialized_copy(first, last, begin_);
	}

	template<typename T, typename Alloc>
	template<typename InputIt>
	inline void deque<T, Alloc>::range_assign(InputIt first, InputIt last)
	{
		size_type count = mystl::distance(first, last);
		if (size() >= count) {
			////删除多余结点
			erase(mystl::copy(first, last, begin_), end_);
		}
		else {
			auto mid = first;
			mystl::advance(mid, size());
			mystl::copy(first, mid, begin_); //初始化过的区域
			end_ = mystl::uninitialized_copy(mid, last, end_);//未初始化的区域
		}
	}

	template<typename T, typename Alloc>
	void deque<T, Alloc>::ensure_capacity(size_type count, bool front)
	{
		//begin_能否往前移count个位置
		if (front && static_cast<size_type>(begin_.curr - begin_.first) < count) {
			//begin_前是否还有足够可用buffer
			size_type need_buffer = (count - (begin_.curr - begin_.first)) / buffer_size + 1;
			if (need_buffer > static_cast<size_type>(begin_.node - map_)) {
				//扩容中控区
				reallocate_map(need_buffer, front);
			}
			//给buffer分配空间
			for (auto curr = begin_.node - need_buffer; curr < begin_.node; ++curr) {
				*curr = data_allocator::allocate(buffer_size);
			}
		}
		//end_能否往后移count个位置
		else if (!front && static_cast<size_type>(end_.last - end_.curr - 1) < count) {
			//end_后是否还有足够可用buffer
			size_type need_buffer = (count - (end_.last - end_.curr - 1)) / buffer_size + 1;
			if (need_buffer > static_cast<size_type>((map_ + map_size_) - end_.node - 1)) {
				reallocate_map(need_buffer, front);
			}
			//给buffer分配空间
			for (auto curr = end_.node + 1; curr <= end_.node + need_buffer; ++curr) {
				*curr = data_allocator::allocate(buffer_size);
			}
		}
	}

	template<typename T, typename Alloc>
	void deque<T, Alloc>::reallocate_map(size_type need_buffer, bool front)
	{
		//中控区大小至少扩容两倍
		size_type new_map_size = map_size_ + mystl::max(map_size_, need_buffer + 2);
		map_pointer new_map = allocate_map(new_map_size);

		size_type old_buffer = end_.node - begin_.node + 1;
		size_type new_buffer = old_buffer + need_buffer;

		//给前半部分或后半部分空出need_buffer个缓冲区
		map_pointer new_begin = new_map + (new_map_size - new_buffer) / 2
			+ (front ? need_buffer : 0);
		//将缓冲区的指针复制给新的中控区
		mystl::copy(begin_.node, end_.node + 1, new_begin);

		map_allocator::deallocate(map_, map_size_);
		map_ = new_map;
		map_size_ = new_map_size;

		//只是node改变了
		begin_.node = new_begin;
		end_.node = new_begin + old_buffer - 1;
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::free()
	{
		if (map_ != nullptr) {
			clear();
			data_allocator::deallocate(begin_.first, buffer_size);
			map_allocator::deallocate(map_, map_size_);
		}
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::deque()
	{
		map_initialize(0);
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::deque(size_type count)
	{
		fill_initialize(count, value_type());
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::deque(size_type count, const T& value)
	{
		fill_initialize(count, value);
	}

	template<typename T, typename Alloc>
	template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline deque<T, Alloc>::deque(InputIt first, InputIt last)
	{
		range_initialize(first, last);
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::deque(const deque& other)
	{
		range_initialize(other.begin(), other.end());
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::deque(deque&& other) noexcept
		:begin_(std::move(other.begin_)),
		end_(std::move(other.end_)),
		map_(other.map_),
		map_size_(other.map_size_)
	{
		other.map_ = nullptr;
		other.map_size_ = 0;
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::deque(std::initializer_list<T> init)
	{
		range_initialize(init.begin(), init.end());
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>::~deque()
	{
		free();
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& other)
	{
		if (this != &other) {
			range_assign(other.begin(), other.end());
		}
		return *this;
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>& deque<T, Alloc>::operator=(deque&& other) noexcept
	{
		if (this != &other) {
			free();
			begin_ = std::move(other.begin_);
			end_ = std::move(other.end_);
			map_ = other.map_;
			map_size_ = other.map_size_;
			other.map_ = nullptr;
			other.map_size_ = 0;
		}
		return *this;
	}

	template<typename T, typename Alloc>
	inline deque<T, Alloc>& deque<T, Alloc>::operator=(std::initializer_list<T> ilist)
	{
		range_assign(ilist.begin(), ilist.end());
		return *this;
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::assign(size_type count, const T& value)
	{
		if (count > size()) {
			mystl::fill(begin(), end(), value);
			insert(end(), count - size(), value);//插入新节点
		}
		else { //删除多余节点
			erase(begin() + count, end());
			mystl::fill(begin(), end(), value);
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void deque<T, Alloc>::assign(InputIt first, InputIt last)
	{
		range_assign(first, last);
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::assign(std::initializer_list<T> ilist)
	{
		range_assign(ilist.begin(), ilist.end());
	}


	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::shrink_to_fit()
	{
		//释放掉[begin_.node,end_.node]以外的缓冲区
		for (auto curr = map_; curr < begin_.node; ++curr) {
			data_allocator::deallocate(*curr, buffer_size);
			*curr = nullptr;
		}
		for (auto curr = end_.node + 1; curr < map_ + map_size_; ++curr) {
			data_allocator::deallocate(*curr, buffer_size);
			*curr = nullptr;
		}
	}

	template<typename T, typename Alloc>
	void deque<T, Alloc>::clear() noexcept
	{
		//先处理头尾之外的缓冲区(因为头尾的元素可能没有填满整个缓冲区)
		for (auto curr = begin_.node + 1; curr < end_.node; ++curr) {
			mystl::destroy(*curr, *curr + buffer_size);
		}
		//头尾缓冲区不同，则头缓冲区一定填满了
		if (begin_.node != end_.node) {
			mystl::destroy(begin_.curr, begin_.last);
			mystl::destroy(end_.first, end_.curr);
		}
		//只有头缓冲区有元素
		else {
			mystl::destroy(begin_.curr, end_.curr);
		}

		shrink_to_fit();
		//留下begin_指向的缓冲区
		for (auto curr = begin_.node + 1; curr <= end_.node; ++curr) {
			data_allocator::deallocate(*curr, buffer_size);
			*curr = nullptr;
		}
		end_ = begin_;
	}

	template<typename T, typename Alloc>
	inline typename deque<T, Alloc>::iterator
		deque<T, Alloc>::insert(const_iterator pos, const T& value)
	{
		return emplace(pos, value);
	}

	template<typename T, typename Alloc>
	inline typename deque<T, Alloc>::iterator
		deque<T, Alloc>::insert(const_iterator pos, T&& value)
	{
		return emplace(pos, std::move(value));
	}

	template<typename T, typename Alloc>
	typename deque<T, Alloc>::iterator
		deque<T, Alloc>::fill_insert(const_iterator pos, size_type count, const T& value)
	{
		iterator ret; //insert之后构造返回值，因为insert可能使迭代器失效
		size_type pos_before_elems = pos - begin_;
		if (pos_before_elems < size() / 2) {
			//在头部构造，然后旋转
			insert(begin_, count, value);
			ret = begin_ + pos_before_elems + count;
			mystl::rotate(begin_, begin_ + count, ret);
			ret -= count;
		}
		else {
			//在尾部构造，然后旋转
			insert(end_, count, value);
			ret = begin_ + pos_before_elems;
			mystl::rotate(ret, end_ - count, end_);
		}
		return ret;
	}

	template<typename T, typename Alloc>
	inline typename deque<T, Alloc>::iterator
		deque<T, Alloc>::insert(const_iterator pos, size_type count, const T& value)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end());
		if (count == 0)
			return iterator(pos.curr, pos.node);
		iterator ret;
		//在头部插入
		if (pos.curr == begin_.curr) {
			ensure_capacity(count, true);
			auto new_begin = begin_ - count;
			mystl::uninitialized_fill_n(new_begin, count, value);
			begin_ = new_begin;
			return begin_;
		}
		//在尾部插入
		else if (pos.curr == end_.curr) {
			ensure_capacity(count, false);
			iterator ret = end_;
			end_ = mystl::uninitialized_fill_n(end_, count, value);
			return ret;
		}
		//在中间位置插入
		else {
			return fill_insert(pos, count, value);
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIt>
	typename deque<T, Alloc>::iterator
		deque<T, Alloc>::range_insert(const_iterator pos, InputIt first, InputIt last)
	{
		iterator ret; //insert之后构造返回值，因为insert可能使迭代器失效
		size_type count = mystl::distance(first, last);
		size_type pos_before_elems = pos - begin_;
		if (pos_before_elems < size() / 2) {
			//在头部构造，然后旋转
			insert(begin_, first, last);
			ret = begin_ + pos_before_elems + count;
			mystl::rotate(begin_, begin_ + count, ret);
			ret -= count;
		}
		else {
			//在尾部构造，然后旋转
			insert(end_, first, last);
			ret = begin_ + pos_before_elems;
			mystl::rotate(ret, end_ - count, end_);
		}
		return ret;
	}

	template<typename T, typename Alloc>
	template< typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	typename deque<T, Alloc>::iterator
		deque<T, Alloc>::insert(const_iterator pos, InputIt first, InputIt last)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end() && first <= last);
		if (first == last)
			return iterator(pos.curr, pos.node);
		size_type count = mystl::distance(first, last);
		// 在头部插入
		if (pos.curr == begin_.curr) {
			ensure_capacity(count, true);
			auto new_begin = begin_ - count;
			mystl::uninitialized_copy(first, last, new_begin);
			begin_ = new_begin;
			return begin_;
		}
		//在尾部插入
		else if (pos.curr == end_.curr) {
			ensure_capacity(count, false);
			iterator ret = end_;
			end_ = mystl::uninitialized_copy(first, last, end_);
			return ret;
		}
		//在中间位置插入
		else {
			return range_insert(pos, first, last);
		}
	}

	template<typename T, typename Alloc>
	inline typename deque<T, Alloc>::iterator
		deque<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
	{
		return insert(ilist.begin(), ilist.end());
	}

	template<typename T, typename Alloc>
	template<class ...Args>
	typename deque<T, Alloc>::iterator
		deque<T, Alloc>::__insert(const_iterator pos, Args && ...args)
	{
		iterator ret;//insert之后构造返回值，因为insert可能使迭代器失效
		size_type pos_before_elems = pos - begin_;
		if (pos_before_elems < size() / 2) {
			//在头部构造，然后旋转
			emplace_front(std::forward<Args>(args)...);
			ret = begin_ + pos_before_elems + 1;
			mystl::rotate(begin_, begin_ + 1, ret);
			--ret;
		}
		else {
			//在尾部构造，然后旋转
			emplace_back(std::forward<Args>(args)...);
			ret = begin_ + pos_before_elems;
			mystl::rotate(ret, end_ - 1, end_);
		}
		return ret;
	}

	template<typename T, typename Alloc>
	template<class ...Args>
	inline typename deque<T, Alloc>::iterator
		deque<T, Alloc>::emplace(const_iterator pos, Args && ...args)
	{
		ASSERT_EXPR(pos >= begin() && pos <= end());
		//在头部插入
		if (pos.curr == begin_.curr) {
			emplace_front(std::forward<Args>(args)...);
			return begin_;
		}
		//在尾部插入
		else if (pos.curr == end_.curr) {
			emplace_back(std::forward<Args>(args)...);
			return end_ - 1;
		}
		//在中间插入
		else {
			return __insert(pos, std::forward<Args>(args)...);
		}
	}

	template<typename T, typename Alloc>
	inline typename deque<T, Alloc>::iterator
		deque<T, Alloc>::erase(const_iterator pos)
	{
		ASSERT_EXPR(pos >= begin() && pos < end());
		iterator ret(pos.curr, pos.node);
		iterator next = ret + 1;
		size_type pos_before_elems = pos - begin_;
		//移动剩余元素较小的一边
		if (pos_before_elems < (size() / 2)) {
			mystl::move_backward(begin_, ret, next);
			pop_front();
		}
		else {
			mystl::move(next, end_, ret);
			pop_back();
		}
		return ret;
	}

	template<typename T, typename Alloc>
	typename deque<T, Alloc>::iterator
		deque<T, Alloc>::erase(const_iterator first, const_iterator last)
	{
		ASSERT_EXPR(first >= begin() && last <= end() && first <= last);
		iterator ret(last.curr, last.node);
		if (first == begin() && last == end()) {
			clear();
		}
		else {
			size_type count = last - first;
			size_type first_before_elems = first - begin_;
			//移动剩余元素较小的一边
			if (first_before_elems < (size() - count) / 2) {
				//[begin_,first)移动到[last - (first - begin_),last)
				mystl::move_backward(begin_, iterator(first.curr, first.node), ret);
				auto new_begin = begin_ + count;
				mystl::destroy(begin_, new_begin);
				//清除多余缓冲区
				for (auto curr = begin_.node; curr < new_begin.node; ++curr) {
					data_allocator::deallocate(*curr, buffer_size);
					*curr = nullptr;
				}
				begin_ = new_begin;
			}
			else {
				//[last,end_)移动到[first,first + (end_ - last));
				mystl::move(ret, end_, iterator(first.curr, first.node));
				auto new_end = end_ - count;
				mystl::destroy(new_end, end_);
				//清除多余缓冲区
				for (auto curr = new_end.node + 1; curr <= end_.node; ++curr) {
					data_allocator::deallocate(*curr, buffer_size);
					*curr = nullptr;
				}
				end_ = new_end;
			}
		}

		return ret;
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::push_back(const T& value)
	{
		emplace_back(value);
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::push_back(T&& value)
	{
		emplace_back(std::move(value));
	}


	template<typename T, typename Alloc>
	template<class ...Args>
	inline void deque<T, Alloc>::emplace_back(Args && ...args)
	{
		//至少要有两个两个空间
		//考虑当前end_所指缓冲区，是最后一个
		//++end_会引起错误，因为他会移动到下一个缓冲区，但是根本就没有下一个
		if (end_.curr != end_.last - 1) {
			mystl::construct(end_.curr, std::forward<Args>(args)...);
			++end_;
		}
		else {
			ensure_capacity(1, false);
			mystl::construct(end_.curr, std::forward<Args>(args)...);
			++end_;
		}
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::pop_back()
	{
		ASSERT_EXPR(!empty());
		//尾部缓冲区有元素
		if (end_.curr != end_.first) {
			--end_;
			mystl::destroy_at(end_.curr);
		}
		//尾部缓冲区没有元素，清除缓冲区
		else {
			--end_;
			mystl::destroy_at(end_.curr);
			data_allocator::deallocate(*(end_.node + 1), buffer_size);
		}
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::push_front(const T& value)
	{
		emplace_front(value);
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::push_front(T&& value)
	{
		emplace_front(std::move(value));
	}


	template<typename T, typename Alloc>
	template<class ...Args>
	inline void deque<T, Alloc>::emplace_front(Args && ...args)
	{
		if (begin_.curr != begin_.first) {
			--begin_;
			mystl::construct(begin_.curr, std::forward<Args>(args)...);
		}
		else {
			ensure_capacity(1, true);
			--begin_;
			mystl::construct(begin_.curr, std::forward<Args>(args)...);
		}
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::pop_front()
	{
		ASSERT_EXPR(!empty());
		//头部缓冲区有至少两个元素
		if (begin_.curr != begin_.last - 1) {
			mystl::destroy_at(begin_.curr);
			++begin_;
		}
		//头部缓冲区只有一个元素，要清除缓冲区
		else {
			mystl::destroy_at(begin_.curr);
			++begin_;
			data_allocator::deallocate(*(begin_.node - 1), buffer_size);
		}
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::resize(size_type count)
	{
		resize(count, value_type());
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::resize(size_type count, const value_type& value)
	{
		size_type sz = size();
		if (count < sz) {
			erase(begin_ + count, end_);
		}
		else if (count > sz) {
			insert(end_, count - sz, value);
		}
	}

	template<typename T, typename Alloc>
	inline void deque<T, Alloc>::swap(deque& other) noexcept
	{
		if (this != &other) {
			mystl::swap(begin_, other.begin_);
			mystl::swap(end_, other.end_);
			mystl::swap(map_, other.map_);
			mystl::swap(map_size_, other.map_size_);
		}
	}

	//非成员函数
	template< class T, class Alloc >
	bool operator==(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs) {
		return lhs.size() == rhs.size() &&
			mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}
	template< class T, class Alloc >
	bool operator!=(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template< class T, class Alloc >
	bool operator<(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs) {
		return mystl::lexicographical_compare(
			lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}
	template< class T, class Alloc >
	bool operator<=(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs) {
		return !(rhs < lhs);
	}
	template< class T, class Alloc >
	bool operator>(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs) {
		return rhs < lhs;
	}
	template< class T, class Alloc >
	bool operator>=(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	// 重载 mystl 的 swap
	template <class T, class Alloc>
	void swap(deque<T, Alloc>& lhs, 
		deque<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}



}
#endif // !_MY_DEQUE_H_
