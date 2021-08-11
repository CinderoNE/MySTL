#pragma once
#ifndef _MY_MAP_H_
#define _MY_MAP_H_
#include"my_rb_tree.h"
#include"my_exceptdef.h"

/*
此头文件包括map和mutlimap
*/
namespace mystl {

	template<typename Key,typename Value,typename Compare = std::less<Key>,typename Alloc = mystl::allocator<std::pair<const Key,Value>>>
	class map {
	public:
		using key_type = Key;
		using mapped_type = Value;
		using value_type = std::pair<Key, Value>;
		using key_compare = Compare;

		struct value_compare {
		private:
			Compare comp;
			value_compare(Compare c) :comp(c) {}
		public:
			bool operator()(const value_type& lhs, const value_type& rhs) const
			{
				return comp(lhs.first, rhs.first);
			}
		};
	private:
		//底层数据结构
		using data_structure = mystl::rb_tree<value_type, Compare, Alloc>;

		data_structure t_;
	public:
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = typename data_structure::iterator;
		using const_iterator = typename data_structure::const_iterator;
		using reverse_iterator = typename data_structure::reverse_iterator;
		using const_reverse_iterator = typename data_structure::const_reverse_iterator;

		allocator_type get_allocator() const { return t_.get_allocator(); }
		key_compare key_comp() const { return t_.key_comp(); }
		value_compare value_comp() const { value_compare(t_.key_comp_); }
	public:
		//构造函数
		map() :t_(Compare()) {}

		explicit map(const Compare& comp) :t_(comp) {}

		template< typename InputIt >
		map(InputIt first, InputIt last, const Compare& comp = Compare())
			: t_(comp)
		{
			t_.insert_unique(first, last);
		}

		map(std::initializer_list<value_type> init, const Compare& comp = Compare())
			:t_(comp)
		{
			t_.insert_unique(init.begin(), init.end());
		}

		map(const map& other) :t_(other.t_) {}

		map(map&& other)noexcept :t_(std::move(other.t_)) {}
		//析构
		~map() = default;

		//赋值运算符
		map& operator=(const map& other)
		{
			t_ = other.t_;
			return *this;
		}

		map& operator=(map&& other) noexcept
		{
			t_ = std::move(other.t_);
			return *this;
		}

		map& operator=(std::initializer_list<value_type> ilist)
		{
			clear();
			t_.insert_unique(ilist.begin(), ilist.end());
			return *this;
		}


		//元素访问
		Value& at(const Key& key)
		{
			iterator it = find(key);
			ASSERT_EXPR(it != end());
			return it->second;
			
		}
		const Value& at(const Key& key) const
		{
			iterator it = find(key);
			ASSERT_EXPR(it != end());
			return it->second;
		}


		Value& operator[](const Key& key)
		{
			iterator it = find(key);
			if (it == end()) {
				return emplace(key, Value()).first->second;
			}
			else {
				return it->second;
			}
		}

		Value& operator[](Key&& key) {
			iterator it = find(key);
			if (it == end()) {
				return emplace(std::move(key), Value()).first->second;
			}
			else {
				return it->second;
			}
		}

	public:
		//迭代器
		iterator begin() noexcept { return t_.begin(); }
		const_iterator begin() const noexcept { return t_.begin(); }
		const_iterator cbegin() const noexcept { return t_.cbegin(); }
		iterator end() noexcept { return t_.end(); }
		const_iterator end() const noexcept { return t_.end(); }
		const_iterator cend() const noexcept { return t_.cend(); }

		reverse_iterator rbegin() noexcept { return t_.rbegin(); }
		const_reverse_iterator rbegin() const noexcept { return t_.rbegin(); }
		const_reverse_iterator crbegin() const noexcept { return t_.crbegin(); }
		reverse_iterator rend() noexcept { return t_.rend(); }
		const_reverse_iterator rend() const noexcept { return t_.rend(); }
		const_reverse_iterator crend() const noexcept { return t_.crend(); }

		//容量
		bool empty() const noexcept { return t_.empty(); }
		size_type size() const noexcept { return t_.size(); }

		//修改器
		void clear() noexcept { t_.clear(); }

		//insert
		std::pair<iterator, bool> insert(const value_type& value)
		{
			return t_.insert_unique(value);
		}
		std::pair<iterator, bool> insert(value_type&& value)
		{
			return t_.insert_unique(std::move(value));
		}
		iterator insert(const_iterator hint, const value_type& value)
		{
			return t_.insert_unique(hint, value);
		}
		iterator insert(const_iterator hint, value_type&& value)
		{
			return t_.insert_unique(hint, std::move(value));
		}
		template< typename InputIt >
		void insert(InputIt first, InputIt last)
		{
			t_.insert_unique(first, last);
		}
		void insert(std::initializer_list<value_type> ilist)
		{
			t_.insert_unique(ilist.begin(), ilist.end());
		}

		//emplace
		template< class... Args >
		std::pair<iterator, bool> emplace(Args&&... args)
		{
			return t_.emplace_unique(std::forward<Args>(args)...);
		}

		template <class... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args)
		{
			return t_.emplace_unique_hint(hint, std::forward<Args>(args)...);
		}

		//erase
		iterator erase(const_iterator pos)
		{
			return t_.erase(pos);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			return t_.erase(first, last);
		}

		size_type erase(const key_type& key)
		{
			return t_.erase_unique(key);
		}

		void swap(map& rhs) noexcept
		{
			t_.swap(rhs.t_);
		}

		//查找
		size_type count(const Key& key) const
		{
			return t_.count(key);
		}

		iterator find(const Key& key)
		{
			return t_.find(key);
		}

		const_iterator find(const Key& key) const
		{
			return t_.find(key);
		}

		std::pair<iterator, iterator> equal_range(const Key& key)
		{
			return t_.equal_range(key);
		}

		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const
		{
			return t_.equal_range(key);
		}

		iterator lower_bound(const Key& key)
		{
			return t_.lower_bound(key);
		}
		const_iterator lower_bound(const Key& key) const
		{
			return t_.lower_bound(key);
		}

		iterator upper_bound(const Key& key)
		{
			return t_.upper_bound(key);
		}

		const_iterator upper_bound(const Key& key) const
		{
			return t_.upper_bound(key);
		}

	public:
		friend bool operator==(const map& lhs, const map& rhs) { return lhs.t_ == rhs.t_; }
		friend bool operator< (const map& lhs, const map& rhs) { return lhs.t_ < rhs.t_; }
	};

	//非成员函数
	template< class Key, class Value, class Compare, class Alloc >
	bool operator==(const mystl::map<Key, Value, Compare, Alloc>& lhs, const mystl::map<Key, Value, Compare, Alloc>& rhs)
	{
		return lhs == rhs;
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator!=(const mystl::map<Key, Value, Compare, Alloc>& lhs, const mystl::map<Key, Value, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator<(const mystl::map<Key, Value, Compare, Alloc>& lhs, const mystl::map<Key, Value, Compare, Alloc>& rhs)
	{
		return lhs < rhs;
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator<=(const mystl::map<Key, Value, Compare, Alloc>& lhs, const mystl::map<Key, Value, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator>(const mystl::map<Key, Value, Compare, Alloc>& lhs, const mystl::map<Key, Value, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator>=(const mystl::map<Key, Value, Compare, Alloc>& lhs, const mystl::map<Key, Value, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}


	template< class Key, class Value, class Compare, class Alloc >
	void swap(mystl::map<Key, Value, Compare, Alloc>& lhs, mystl::map<Key, Value, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}


	/***************************************************************************
	*				               multimap                                    *
	***************************************************************************/
	template<typename Key, typename Value, typename Compare = std::less<Key>, typename Alloc = mystl::allocator<std::pair<const Key, Value>>>
	class multimap {
	public:
		using key_type = Key;
		using mapped_type = Value;
		using value_type = std::pair<Key, Value>;
		using key_compare = Compare;

		struct value_compare {
		private:
			Compare comp;
			value_compare(Compare c) :comp(c) {}
		public:
			bool operator()(const value_type& lhs, const value_type& rhs) const
			{
				return comp(lhs.first, rhs.first);
			}
		};
	private:
		//底层数据结构
		using data_structure = mystl::rb_tree<value_type, Compare, Alloc>;

		data_structure t_;
	public:
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = typename data_structure::iterator;
		using const_iterator = typename data_structure::const_iterator;
		using reverse_iterator = typename data_structure::reverse_iterator;
		using const_reverse_iterator = typename data_structure::const_reverse_iterator;

		allocator_type get_allocator() const { return t_.get_allocator(); }
		key_compare key_comp() const { return t_.key_comp(); }
		value_compare value_comp() const { value_compare(t_.key_comp_); }
	public:
		//构造函数
		multimap() :t_(Compare()) {}

		explicit multimap(const Compare& comp) :t_(comp) {}

		template< typename InputIt >
		multimap(InputIt first, InputIt last, const Compare& comp = Compare())
			: t_(comp)
		{
			t_.insert_multi(first, last);
		}

		multimap(std::initializer_list<value_type> init, const Compare& comp = Compare())
			:t_(comp)
		{
			t_.insert_multi(init.begin(), init.end());
		}

		multimap(const multimap& other) :t_(other.t_) {}

		multimap(multimap&& other) noexcept:t_(std::move(other.t_)) {}
		//析构
		~multimap() = default;

		//赋值运算符
		multimap& operator=(const multimap& other)
		{
			t_ = other.t_;
			return *this;
		}

		multimap& operator=(multimap&& other)noexcept
		{
			t_ = std::move(other.t_);
			return *this;
		}

		multimap& operator=(std::initializer_list<value_type> ilist)
		{
			clear();
			t_.insert_multi(ilist.begin(), ilist.end());
			return *this;
		}


		//元素访问
		Value& at(const Key& key)
		{
			iterator it = find(key);
			ASSERT_EXPR(it != end());
			return it->second;

		}
		const Value& at(const Key& key) const
		{
			iterator it = find(key);
			ASSERT_EXPR(it != end());
			return it->second;
		}


		Value& operator[](const Key& key)
		{
			iterator it = find(key);
			if (it == end()) {
				return emplace(std::make_pair(key, Value())).first->second;
			}
			else {
				return it->second;
			}
		}

		Value& operator[](Key&& key) {
			iterator it = find(key);
			if (it == end()) {
				return emplace(std::make_pair(std::move(key), Value())).first->second;
			}
			else {
				return it->second;
			}
		}

	public:
		//迭代器
		iterator begin() noexcept { return t_.begin(); }
		const_iterator begin() const noexcept { return t_.begin(); }
		const_iterator cbegin() const noexcept { return t_.cbegin(); }
		iterator end() noexcept { return t_.end(); }
		const_iterator end() const noexcept { return t_.end(); }
		const_iterator cend() const noexcept { return t_.cend(); }

		reverse_iterator rbegin() noexcept { return t_.rbegin(); }
		const_reverse_iterator rbegin() const noexcept { return t_.rbegin(); }
		const_reverse_iterator crbegin() const noexcept { return t_.crbegin(); }
		reverse_iterator rend() noexcept { return t_.rend(); }
		const_reverse_iterator rend() const noexcept { return t_.rend(); }
		const_reverse_iterator crend() const noexcept { return t_.crend(); }

		//容量
		bool empty() const noexcept { return t_.empty(); }
		size_type size() const noexcept { return t_.size(); }

		//修改器
		void clear() noexcept { t_.clear(); }

		//insert
		iterator insert(const value_type& value)
		{
			return t_.insert_multi(value);
		}
		iterator insert(value_type&& value)
		{
			return t_.insert_multi(std::move(value));
		}
		iterator insert(const_iterator hint, const value_type& value)
		{
			return t_.insert_multi(hint, value);
		}
		iterator insert(const_iterator hint, value_type&& value)
		{
			return t_.insert_multi(hint, std::move(value));
		}
		template< typename InputIt >
		void insert(InputIt first, InputIt last)
		{
			t_.insert_multi(first, last);
		}
		void insert(std::initializer_list<value_type> ilist)
		{
			t_.insert_multi(ilist.begin(), ilist.end());
		}

		//emplace
		template< class... Args >
		iterator emplace(Args&&... args)
		{
			return t_.emplace_multi(std::forward<Args>(args)...);
		}

		template <class... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args)
		{
			return t_.emplace_multi_hint(hint, std::forward<Args>(args)...);
		}

		//erase
		iterator erase(const_iterator pos)
		{
			return t_.erase(pos);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			return t_.erase(first, last);
		}

		size_type erase(const key_type& key)
		{
			return t_.erase_multi(key);
		}

		void swap(multimap& rhs) noexcept
		{
			t_.swap(rhs.t_);
		}

		//查找
		size_type count(const Key& key) const
		{
			return t_.count(key);
		}

		iterator find(const Key& key)
		{
			return t_.find(key);
		}

		const_iterator find(const Key& key) const
		{
			return t_.find(key);
		}

		std::pair<iterator, iterator> equal_range(const Key& key)
		{
			return t_.equal_range(key);
		}

		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const
		{
			return t_.equal_range(key);
		}

		iterator lower_bound(const Key& key)
		{
			return t_.lower_bound(key);
		}
		const_iterator lower_bound(const Key& key) const
		{
			return t_.lower_bound(key);
		}

		iterator upper_bound(const Key& key)
		{
			return t_.upper_bound(key);
		}

		const_iterator upper_bound(const Key& key) const
		{
			return t_.upper_bound(key);
		}

	public:
		friend bool operator==(const multimap& lhs, const multimap& rhs) { return lhs.t_ == rhs.t_; }
		friend bool operator< (const multimap& lhs, const multimap& rhs) { return lhs.t_ < rhs.t_; }
	};


	//非成员函数
	template< class Key, class Value, class Compare, class Alloc >
	bool operator==(const mystl::multimap<Key, Value, Compare, Alloc>& lhs, const mystl::multimap<Key, Value, Compare, Alloc>& rhs)
	{
		return lhs == rhs;
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator!=(const mystl::multimap<Key, Value, Compare, Alloc>& lhs, const mystl::multimap<Key, Value, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator<(const mystl::multimap<Key, Value, Compare, Alloc>& lhs, const mystl::multimap<Key, Value, Compare, Alloc>& rhs)
	{
		return lhs < rhs;
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator<=(const mystl::multimap<Key, Value, Compare, Alloc>& lhs, const mystl::multimap<Key, Value, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator>(const mystl::multimap<Key, Value, Compare, Alloc>& lhs, const mystl::multimap<Key, Value, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template< class Key, class Value, class Compare, class Alloc >
	bool operator>=(const mystl::multimap<Key, Value, Compare, Alloc>& lhs, const mystl::multimap<Key, Value, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}


	template< class Key, class Value, class Compare, class Alloc >
	void swap(mystl::multimap<Key, Value, Compare, Alloc>& lhs, mystl::multimap<Key, Value, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

} //end of namespace mystl


#endif // !_MY_MAP_H_
