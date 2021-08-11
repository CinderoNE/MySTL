#pragma once
#ifndef _MY_LIST_H_
#define _MY_LIST_H_

#include"my_allocator.h"
#include"my_iterator.h"
#include"my_exceptdef.h"


namespace mystl {

	struct list_node_base {
		list_node_base() :prev(this), next(this)
		{
		}
		list_node_base* prev;
		list_node_base* next;
	};

	template<typename T>
	struct list_node :public list_node_base {
		using node_ptr = list_node<T>*;
		T value;

		list_node(const T& val)
			:value(val)
		{
		}

		list_node(T&& val)
			:value(std::move(val))
		{
		}
	};

	template<typename T>
	struct list_const_iterator :public mystl::iterator<mystl::bidirectional_iterator_tag, T> {
		using node_ptr = typename list_node<T>::node_ptr;
		using base_ptr = list_node_base*;
		using self = list_const_iterator<T>;

		using value_type = T;
		using pointer = const T*;
		using reference = const T&;

		base_ptr base_p;

		list_const_iterator() :base_p(nullptr) {}
		list_const_iterator(base_ptr p) :base_p(p) {}

		reference operator*() const noexcept {
			return static_cast<node_ptr>(base_p)->value;
		}

		pointer operator->() const noexcept {
			return &(operator*());
		}

		self& operator++() noexcept {
			ASSERT_EXPR(base_p != nullptr);
			base_p = base_p->next;
			return *this;
		}

		self operator++(int) noexcept {
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--() noexcept {
			ASSERT_EXPR(base_p != nullptr);
			base_p = base_p->prev;
			return *this;
		}

		self operator--(int) noexcept {
			self tmp = *this;
			--* this;
			return tmp;
		}

		bool operator==(const self& rhs) const noexcept {
			return base_p == rhs.base_p;
		}


		bool operator!=(const self& rhs) const noexcept {
			return !(*this == rhs);
		}
		
	};

	template<typename T>
	struct list_iterator :public list_const_iterator<T> {
		using node_ptr = typename list_node<T>::node_ptr;
		using base_ptr = list_node_base*;
		using self = list_iterator<T>;

		using value_type = T;
		using pointer = T*;
		using reference = T&;

		using list_const_iterator<T>::base_p; //使用基类的成员

		list_iterator() = default;

		list_iterator(base_ptr p) :list_const_iterator<T>(p) {}

		reference operator*() const noexcept {
			return static_cast<node_ptr>(base_p)->value;
		}

		pointer operator->() const noexcept {
			return &(operator*());
		}

		self& operator++() noexcept {
			ASSERT_EXPR(base_p != nullptr);
			base_p = base_p->next;
			return *this;
		}

		self operator++(int) noexcept {
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--() noexcept {
			ASSERT_EXPR(base_p != nullptr);
			base_p = base_p->prev;
			return *this;
		}

		self operator--(int) noexcept {
			self tmp = *this;
			--* this;
			return tmp;
		}
	};


	template<typename T, typename Alloc = mystl::allocator<T>>
	class list {
	public:
		using value_type = T;
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = list_iterator<T>;
		using const_iterator = list_const_iterator<T>;
		using reverse_iterator = mystl::reverse_iterator<iterator>;
		using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

		using base_type = list_node_base;
		using base_ptr = base_type*;
		using node_type = list_node<T>;
		using node_ptr = node_type*;

		using node_allocator = mystl::allocator<node_type>;
		using data_allocator = Alloc;
		allocator_type get_allocator() { return data_allocator(); }

	private:
		base_type node_; //虚拟节点,&node_ == end ,next == begin
		size_type size_;

	private:
		template<typename... Args>
		node_ptr create_node(Args&&...args);
		void destory_node(node_ptr ptr);
		void link_back(base_ptr ptr);
		void link_front(base_ptr ptr);
		iterator link_at_pos(const_iterator pos, base_ptr ptr);

		void fill_initialize(size_type count, const T& value);
		template<typename InputIt>
		void range_initialize(InputIt first, InputIt last);
		template<typename InputIt>
		void range_assign(InputIt first, InputIt last);

	public:
		list();//默认构造  
		explicit list(size_type count); //count个默认值
		list(size_type count, const T& value);//count个value值
		template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		list(InputIt first, InputIt last);//范围构造
		list(std::initializer_list<T> init);//初始化列表
		list(const list& other);//拷贝构造
		list(list&& other) noexcept;//移动构造
		~list();//析构

		list& operator=(const list& other);
		list& operator=(list&& other) noexcept;
		list& operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T& value);
		template< class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		void assign(InputIt first, InputIt last);
		void assign(std::initializer_list<T> ilist);
	public:
		//元素访问
		reference front() 
		{ 
			ASSERT_EXPR(!empty()); 
			return static_cast<node_ptr>(node_.prev)->value;
		}
		const_reference front() const 
		{ 
			ASSERT_EXPR(!empty()); 
			return static_cast<node_ptr>(node_.prev)->value;
		}
		reference back() 
		{ 
			ASSERT_EXPR(!empty()); 
			return static_cast<node_ptr>(node_.prev)->value; 
		}
		const_reference back() const 
		{ 
			ASSERT_EXPR(!empty()); 
			return static_cast<node_ptr>(node_.prev)->value;
		}

		//迭代器
		iterator begin() noexcept { return node_.next; }
		const_iterator begin() const noexcept { return node_.next; }
		const_iterator cbegin() const noexcept { return node_.next; }
		iterator end() noexcept { return &node_; }
		const_iterator end() const noexcept { return const_cast<base_ptr>(&node_); }
		const_iterator cend() const noexcept { return const_cast<base_ptr>(&node_); }

		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
		const_reverse_iterator crbegin() const noexcept { return reverse_iterator(end()); }
		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator crend() const noexcept { return reverse_iterator(begin()); }
	
		//容量
		bool empty() const noexcept { return size_ == 0; }
		size_type size() const noexcept { return size_; }	
	public:
		//修改器
		void clear() noexcept;

	
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

		void pop_back();

		void push_front(const T& value);
		void push_front(T&& value);

		template< class... Args >
		void emplace_front(Args&&... args);

		void pop_front();
		
		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(list& other) noexcept;

	private:
		template<class Compare>
		void __merge(list& other, Compare comp);
		
		template<class Compare>
		void __sort(Compare comp);

	public:
		//操作
		//归并二个已排序链表为一个。链表应以升序排序。
		//不复制元素。操作后容器 other 变为空。若 other 与* this 指代同一对象则函数不做任何事。
		void merge(list& other);
		void merge(list&& other);
		template <class Compare>
		void merge(list& other, Compare comp);
		template <class Compare>
		void merge(list&& other, Compare comp);

		//从一个 list 转移元素给另一个。
		//不复制或移动元素，仅重指向链表结点的内部指针。
		void splice(const_iterator pos, list& other);
		void splice(const_iterator pos, list&& other);
		void splice(const_iterator pos, list& other, const_iterator it);
		void splice(const_iterator pos, list&& other, const_iterator it);
		void splice(const_iterator pos, list& other,
			const_iterator first, const_iterator last);
		void splice(const_iterator pos, list&& other,
			const_iterator first, const_iterator last);

		//移除所有满足特定标准的元素
		void remove(const T& value);
		template< class UnaryPredicate >
		void remove_if(UnaryPredicate pred);

		//逆转容器中的元素顺序
		void reverse() noexcept;

		//从容器移除所有相继的重复元素。只留下相等元素组中的第一个元素。
		void unique();
		template< class BinaryPredicate >
		void unique(BinaryPredicate pred);

		//排序，默认使用operator<
		void sort();
		template< class Compare >
		void sort(Compare comp);

	};

	template<typename T, typename Alloc>
	template<typename... Args>
	inline typename list<T, Alloc>::node_ptr
		list<T, Alloc>::create_node(Args && ...args)
	{
		node_ptr node_p = node_allocator::allocate(1);
		mystl::construct(std::addressof(node_p->value), std::forward<Args>(args)...);
		node_p->prev = nullptr;
		node_p->next = nullptr;
		return node_p;

	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::destory_node(node_ptr ptr)
	{
		mystl::destroy_at(std::addressof(ptr->value));
		node_allocator::deallocate(ptr, 1);
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::link_back(base_ptr ptr)
	{
		ptr->next = &node_;
		ptr->prev = node_.prev;
		node_.prev->next = ptr;
		node_.prev = ptr;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::link_front(base_ptr ptr)
	{
		ptr->next = node_.next;
		node_.next->prev = ptr;
		node_.next = ptr;
		ptr->prev = &node_;
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator 
		list<T, Alloc>::link_at_pos(const_iterator pos, base_ptr ptr)
	{
		auto pos_ptr = pos.base_p;
		if (pos_ptr == &node_) { //在end处插入
			link_back(ptr);
		}
		else if (pos_ptr == node_.next) {//在begin处插入
			link_front(ptr);
		}
		else {
			pos_ptr->prev->next = ptr;
			ptr->prev = pos_ptr->prev;
			pos_ptr->prev = ptr;
			ptr->next = pos_ptr;
		}
		return iterator(ptr);
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::fill_initialize(size_type count, const T& value)
	{
		size_ = count;
		for (; count > 0; --count) {
			auto node_ptr = create_node(value);
			link_back(static_cast<base_ptr>(node_ptr));
		}

	}

	template<typename T, typename Alloc>
	template<typename InputIt>
	inline void list<T, Alloc>::range_initialize(InputIt first, InputIt last)
	{
		size_type count = mystl::distance(first, last);
		size_ = count;
		for (; count > 0; --count, ++first) {
			auto node_ptr = create_node(*first);
			link_back(static_cast<base_ptr>(node_ptr));
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIt>
	void list<T, Alloc>::range_assign(InputIt first, InputIt last)
	{
		auto curr_it = begin();
		auto end_it = end();
		for (; curr_it != end_it && first != last; ++curr_it, ++first) {
			*curr_it = *first;
		}
		if (curr_it == end_it) { //还有结点未复制
			insert(curr_it, first, last);
		}
		else { //删除多余结点
			erase(curr_it, end_it);
		}
	}

	//构造函数
	template<typename T, typename Alloc>
	inline list<T, Alloc>::list()
		:node_(), //prev = next = &node_
		size_(0)
	{
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>::list(size_type count) :list() {
		fill_initialize(count, value_type());
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>::list(size_type count, const T& value) : list()
	{
		fill_initialize(count, value);
	}

	template<typename T, typename Alloc>
	template<class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline list<T, Alloc>::list(InputIt first, InputIt last) :list()
	{
		range_initialize(first, last);
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>::list(std::initializer_list<T> init) :list()
	{
		range_initialize(init.begin(), init.end());
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>::list(const list& other) {
		range_initialize(other.begin(), other.end());
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>::list(list&& other) noexcept
	{
		size_ = other.size_;
		if (other.size_ > 0) {
			node_.prev = other.node_.prev;
			node_.next = other.node_.next;
			node_.prev->next = &node_;
			node_.next->prev = &node_;

			other.node_.prev = other.node_.next = &other.node_;
			other.size_ = 0;
		}
	}

	//析构函数
	template<typename T, typename Alloc>
	inline list<T, Alloc>::~list()
	{
		clear();
	}

	//operator=
	template<typename T, typename Alloc>
	inline list<T, Alloc>& list<T, Alloc>::operator=(const list& other)
	{
		if (this != &other) {
			range_assign(other.begin(), other.end());
		}
		return *this;
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>& list<T, Alloc>::operator=(list&& other) noexcept {
		if (this != &other) {
			clear();
			splice(end(), other);
		}
		return *this;
	}

	template<typename T, typename Alloc>
	inline list<T, Alloc>& list<T, Alloc>::operator=(std::initializer_list<T> ilist) {
		range_assign(ilist.begin(), ilist.end());
		return *this;
	}

	//assign
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::assign(size_type count, const T& value)
	{
		auto b = begin();
		auto e = end();
		for (; count > 0 && b != e; --count, ++b)
		{
			*b = value;
		}
		if (count > 0) //size < count
		{
			insert(e, count, value);
		}
		else if(b != e)//size > count
		{
			erase(b, e);
		}
	}

	template<typename T, typename Alloc>
	template<class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void list<T, Alloc>::assign(InputIt first, InputIt last)
	{
		range_assign(first, last);
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::assign(std::initializer_list<T> ilist) {
		range_assign(ilist.begin(), ilist.end());
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::clear() noexcept
	{
		if (size_ != 0) {
			auto curr_it = begin();
			auto end_it = end();
			for (; curr_it != end_it;) {
				auto curr_ptr = curr_it.base_p;
				++curr_it;
				destory_node(static_cast<node_ptr>(curr_ptr));
			}
			node_.prev = node_.next = &node_;
			size_ = 0;
		}
	}

	//insert
	template<typename T, typename Alloc>
	inline typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(const_iterator pos, const T& value)
	{
		auto ptr = create_node(value);
		++size_;
		return link_at_pos(pos, static_cast<base_ptr>(ptr));
	}

	template<typename T, typename Alloc>
	inline typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(const_iterator pos, T&& value)
	{
		auto ptr = create_node(std::move(value));
		++size_;
		return link_at_pos(pos, static_cast<base_ptr>(ptr));
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(const_iterator pos, size_type count, const T& value)
	{
		iterator ret(pos.base_p);//返回值
		if (count != 0) {
			auto curr_ptr = create_node(value);
			ret.base_p = static_cast<base_ptr>(curr_ptr);
			//连接第一个结点到pos.base_p->prev
			curr_ptr->prev = pos.base_p->prev;
			pos.base_p->prev->next = curr_ptr;

			size_ += count;
			for (--count; count > 0; --count) {
				auto next_ptr = create_node(value);
				curr_ptr->next = next_ptr;
				next_ptr->prev = curr_ptr;
				curr_ptr = next_ptr;
			}
			//连接最后一个节点到pos.base_p
			pos.base_p->prev = curr_ptr;
			curr_ptr->next = pos.base_p;
		}
		return ret;
	}

	template<typename T, typename Alloc>
	template<class InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(const_iterator pos, InputIt first, InputIt last)
	{
		iterator ret(pos.base_p);//返回值
		size_type count = mystl::distance(first, last);
		if (count != 0) {
			auto curr_ptr = create_node(*first);
			ret.base_p = static_cast<base_ptr>(curr_ptr);
			//连接第一个结点到pos_ptr->prev
			auto pos_ptr = pos.base_p;
			curr_ptr->prev = pos_ptr->prev;
			pos_ptr->prev->next = curr_ptr;

			size_ += count;
			for (++first; first != last; ++first) {
				auto next_ptr = create_node(*first);
				curr_ptr->next = next_ptr;
				next_ptr->prev = curr_ptr;
				curr_ptr = next_ptr;
			}
			//连接最后一个节点到pos_ptr
			pos_ptr->prev = curr_ptr;
			curr_ptr->next = pos_ptr;
		}
		return ret;
	}

	template<typename T, typename Alloc>
	inline typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
	{
		insert(ilist.begin(), ilist.end());
	}

	//emplace
	template<typename T, typename Alloc>
	template<class ...Args>
	inline typename list<T, Alloc>::iterator 
		list<T, Alloc>::emplace(const_iterator pos, Args && ...args)
	{
		auto ptr = create_node(std::forward<Args>(args)...);
		++size_;
		return link_at_pos(pos, static_cast<base_ptr>(ptr));
	}

	//erase
	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator 
		list<T, Alloc>::erase(const_iterator pos)
	{
		ASSERT_EXPR(pos != cend());
		auto pos_ptr = pos.base_p;
		iterator ret(pos_ptr->next);
		pos_ptr->prev->next = pos_ptr->next;
		pos_ptr->next->prev = pos_ptr->prev;
		destory_node(static_cast<node_ptr>(pos_ptr));
		--size_;
		return ret;
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(const_iterator first, const_iterator last)
	{
		auto first_ptr = first.base_p;
		auto last_ptr = last.base_p;
		//连接first_ptr->prev和last_ptr
		first_ptr->prev->next = last_ptr;
		last_ptr->prev = first_ptr->prev;

		while (first != last) {
			auto curr_ptr = first.base_p;
			++first;//要在销毁之前++
			destory_node(static_cast<node_ptr>(curr_ptr));
			--size_;
		}
		return iterator(last_ptr);
	}

	//push_back
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::push_back(const T& value)
	{
		auto ptr = create_node(value);
		link_back(static_cast<base_ptr>(ptr));
		++size_;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::push_back(T&& value)
	{
		auto ptr = create_node(std::move(value));
		link_back(static_cast<base_ptr>(ptr));
		++size_;
	}

	template<typename T, typename Alloc>
	template<class ...Args>
	inline void list<T, Alloc>::emplace_back(Args && ...args)
	{
		auto ptr = create_node(std::forward<Args>(args)...);
		link_back(static_cast<base_ptr>(ptr));
		++size_;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::pop_back() 
	{
		ASSERT_EXPR(!empty());

		auto ptr = node_.prev;
		node_.prev = ptr->prev;
		ptr->prev->next = &node_;

		destory_node(static_cast<node_ptr>(ptr));
		--size_;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::push_front(const T& value)
	{
		auto ptr = create_node(value);
		link_front(static_cast<base_ptr>(ptr));
		++size_;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::push_front(T&& value)
	{
		auto ptr = create_node(std::move(value));
		link_front(static_cast<base_ptr>(ptr));
		++size_;
	}

	template<typename T, typename Alloc>
	template<class ...Args>
	inline void list<T, Alloc>::emplace_front(Args && ...args)
	{
		auto ptr = create_node(std::forward<Args>(args)...);
		link_front(static_cast<base_ptr>(ptr));
		++size_;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::pop_front() 
	{
		ASSERT_EXPR(!empty());

		auto ptr = node_.next;
		node_.next = ptr->next;
		ptr->next->prev = &node_;

		destory_node(static_cast<node_ptr>(ptr));
		--size_;
	}

	//resize
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::resize(size_type count) 
	{
		resize(count, value_type());
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::resize(size_type count, const value_type& value)
	{
		if (size_ == count) {
			return;
		}
		else if (size_ > count) {
			auto first = begin();
			for (; count > 0; --count, ++first)
				;
			erase(first, end());
		}
		else { //size_ < count
			insert(end(), count - size_, value);
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::swap(list& other) noexcept
	{
		if (this == &other || (size_ == 0 && other.size_ == 0)) {
			return;
		}
		mystl::swap(size_, other.size_);
		mystl::swap(node_, other.node_);
		//size_为0时，node_.prev和node.next要设置为&node_
		if (size_ == 0) {
			node_.prev = node_.next = &node_;
		}
		else {
			node_.next->prev = node_.prev->next = &node_;
		}
		if (other.size_ == 0) {
			other.node_.prev = other.node_.next = &other.node_;
		}
		else {
			other.node_.next->prev = other.node_.prev->next =  &other.node_;
		}
	}

	//merge(要求两个链表已排序)
	template<typename T, typename Alloc>
	template<class Compare>
	void list<T, Alloc>::__merge(list& other, Compare comp)
	{
		if (this != &other) {
			auto f1 = begin();
			auto l1 = end();
			auto f2 = other.begin();
			auto l2 = other.end();

			base_ptr tmp = &node_;
			//将比较后的链表连接到node_之后
			while (f1 != l1 && f2 != l2) {
				if (comp(*f2, *f1)) { 
					tmp->next = f2.base_p;
					f2.base_p->prev = tmp;
					++f2;
				}
				else { 
					tmp->next = f1.base_p;
					f1.base_p->prev = tmp;
					++f1;
				}
				tmp = tmp->next;
			}
			if (f1 == l1) { //other链表有剩余
				tmp->next = f2.base_p;
				f2.base_p->prev = tmp;
				//将ohter链表的末尾连接到node_
				node_.prev = l2.base_p->prev;
				l2.base_p->prev->next = &node_;
			}
			else {
				tmp->next = f1.base_p;
				f1.base_p->prev = tmp;
				//this链表末尾已经连接
			}
			size_ += other.size_;
			other.size_ = 0;
			other.node_.next = other.node_.prev = &other.node_;
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::merge(list& other)
	{
		__merge(other, std::less<T>());
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::merge(list&& other)
	{
		__merge(other, std::less<T>());
	}

	template<typename T, typename Alloc>
	template<class Compare>
	inline void list<T, Alloc>::merge(list& other, Compare comp)
	{
		__merge(other, comp);
	}

	template<typename T, typename Alloc>
	template<class Compare>
	inline void list<T, Alloc>::merge(list&& other, Compare comp)
	{
		__merge(other, comp);
	}

	//splice
	//从 other 转移所有元素到* this 中。元素被插入到 pos 所指向的元素之前。操作后容器 other 变为空
	//若 other 与 *this 指代同一对象则行为未定义。
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::splice(const_iterator pos, list& other)
	{
		ASSERT_EXPR(this != &other);
		if (!other.empty()) {
			splice(pos, other, other.begin(), other.end());
		}
	}

	
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::splice(const_iterator pos, list&& other)
	{
		splice(pos, other);
	}

	//从 other 转移 it 所指向的元素到 *this 。元素被插入到 pos 所指向的元素之前。
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator it)
	{
		auto pos_ptr = pos.base_p;
		auto it_ptr = it.base_p;
		if (pos_ptr != it_ptr && pos_ptr != it_ptr->next) {
			//与other断开连接
			it_ptr->prev->next = it_ptr->next;
			it_ptr->next->prev = it_ptr->prev;
			link_at_pos(pos, it_ptr);
			++size_;
			--other.size_;
		}
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::splice(const_iterator pos, list&& other, const_iterator it)
	{
		splice(pos, other, it);
	}

	//从 other 转移范围 [first, last) 中的元素到 *this 。元素被插入到 pos 所指向的元素之前。
	//若 pos 是范围 [first,last) 中的迭代器则行为未定义。
	template<typename T, typename Alloc>
	void list<T, Alloc>::splice(const_iterator pos, list& other,
		const_iterator first, const_iterator last)
	{
		if (first != last) {
			auto pos_ptr = pos.base_p;
			size_type count = mystl::distance(first, last);
			auto first_ptr = first.base_p;
			auto last_ptr = last.base_p;
			auto tmp_ptr = last_ptr->prev;//保存最后一个结点

			//与other断开连接
			first_ptr->prev->next = last_ptr;
			last_ptr->prev = first_ptr->prev;

			//first_ptr与pos_ptr->prev连接
			pos_ptr->prev->next = first_ptr;
			first_ptr->prev = pos_ptr->prev;
			//tmp_ptr与pos_ptr连接
			pos_ptr->prev = tmp_ptr;
			tmp_ptr->next = pos_ptr;

			size_ += count;
			other.size_ -= count;
		}
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::splice(const_iterator pos, list&& other,
		const_iterator first, const_iterator last)
	{
		splice(pos, other, first, last);
	}

	//remove
	//移除所有等于 value 的元素
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::remove(const T& value)
	{
		remove_if(
			[&value](const T& val) {return value == val; }
		);
	}

	//移除所有谓词 p 对它返回 true 的元素
	template<typename T, typename Alloc>
	template<class UnaryPredicate>
	inline void list<T, Alloc>::remove_if(UnaryPredicate pred)
	{
		auto curr_it = begin();
		auto end_it = end();
		for (; curr_it != end_it;) {
			if (pred(*curr_it)) {
				curr_it = erase(curr_it);
			}
			else {
				++curr_it;
			}
		}	
	}

	//逆转容器中的元素顺序。不非法化任何引用或迭代器
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::reverse() noexcept
	{
		if (size_ <= 1)
			return;
		auto curr_ptr = begin().base_p;
		auto end_ptr = end().base_p;
		for (; curr_ptr != end_ptr;) {
			mystl::swap(curr_ptr->prev, curr_ptr->next);
			curr_ptr = curr_ptr->prev;
		}
		mystl::swap(end_ptr->prev, end_ptr->next);
	}


	//unique
	//从容器移除所有相继的重复元素。只留下相等元素组中的第一个元素

	//用operator==比较元素
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::unique()
	{
		unique(
			[](const T& v1, const T& v2) {return v1 == v2; }
		);
	}

	//用二元谓词 p 比较元素
	template<typename T, typename Alloc>
	template<class BinaryPredicate>
	void list<T, Alloc>::unique(BinaryPredicate pred)
	{
		if (size_ <= 1)
			return;
		auto curr_it = begin();
		auto end_it = end();
		auto prev_it = curr_it;
		++curr_it;
		while (curr_it != end_it) {
			if (pred(*curr_it, *prev_it)) {
				curr_it = erase(curr_it);
			}
			else {
				prev_it = curr_it;
				++curr_it;
			}
		}
	}


	//sort
	//以升序排序元素。保持相等元素的顺序
	template<typename T,typename Alloc>
	template<class Compare>
	void list<T, Alloc>::__sort(Compare comp)
	{
		//mystl::merge_sort<list<T, Alloc>>(begin(), end());

		//size_ > 1才进行排序
		//归并排序
		//参考g++实现
		if (size_ > 1)
		{
			list carry;
			list tmp[64];
			list* fill = tmp;
			list* counter;
			do
			{
				carry.splice(carry.begin(), *this, begin());
				for (counter = tmp;
					counter != fill && !counter->empty();
					++counter)
				{
					counter->merge(carry,comp);
					carry.swap(*counter);
				}
				carry.swap(*counter);
				if (counter == fill)
					++fill;
			} while (!empty());

			for (counter = tmp + 1; counter != fill; ++counter)
				counter->merge(*(counter - 1), comp);
			swap(*(fill - 1));	
		}
	}

	//用 operator< 比较元素
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::sort()
	{
		__sort(std::less<T>());
	}

	
	//用给定的比较函数 comp 
	template<typename T, typename Alloc>
	template<class Compare>
	inline void list<T, Alloc>::sort(Compare comp)
	{
		__sort(comp);
	}

	
	//非成员函数
	template< class T, class Alloc >
	bool operator==(const list<T, Alloc>& lhs,
		const list<T, Alloc>& rhs) {
		auto f1 = lhs.cbegin();
		auto f2 = rhs.cbegin();
		auto l1 = lhs.cend();
		auto l2 = rhs.cend();
		for (; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
			;
		return f1 == l1 && f2 == l2;
	}

	template< class T, class Alloc >
	bool operator!=(const list<T, Alloc>& lhs,
		const list<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template< class T, class Alloc >
	bool operator<(const list<T, Alloc>& lhs,
		const list<T, Alloc>& rhs) {
		return mystl::lexicographical_compare(
			lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template< class T, class Alloc >
	bool operator<=(const list<T, Alloc>& lhs,
		const list<T, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template< class T, class Alloc >
	bool operator>(const list<T, Alloc>& lhs,
		const list<T, Alloc>& rhs) {
		return rhs < lhs;
	}

	template< class T, class Alloc >
	bool operator>=(const list<T, Alloc>& lhs,
		const list<T, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template< class T, class Alloc >
	void swap(list<T, Alloc>& lhs,
		list<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}



} //end of namespace mystl


#endif // !_MY_LIST_H_
