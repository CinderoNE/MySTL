#ifndef _MY_SET_H_
#define _MY_SET_H_

#include"my_rb_tree.h"

/*
此头文件包括set和mutliset
*/
namespace mystl {

	template<typename Key,typename Compare = std::less<Key>, typename Alloc = mystl::allocator<Key>>
	class set {
	public:
		using key_type = Key;
		using value_type = Key;
		using key_compare = Compare;
		using value_compare = Compare;
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
		key_compare      key_comp()      const { return t_.key_comp(); }
		value_compare    value_comp()    const { return t_.key_comp(); }
	public:
		//构造函数
		set():t_(Compare()){}

		explicit set(const Compare& comp) :t_(comp){}

		template< typename InputIt >
		set(InputIt first, InputIt last, const Compare& comp = Compare())
			: t_(comp)
		{
			t_.insert_unique(first,last);
		}

		set(std::initializer_list<value_type> init, const Compare& comp = Compare())
			:t_(comp)
		{
			t_.insert_unique(init.begin(), init.end());
		}

		set(const set& other) :t_(other.t_) {}

		set(set&& other)noexcept :t_(std::move(other.t_)){}
		//析构
		~set() = default;

		//赋值运算符
		set& operator=(const set& other)
		{
			t_ = other.t_;
			return *this;
		}

		set& operator=(set&& other) noexcept
		{
			t_ = std::move(other.t_);
			return *this;
		}

		set& operator=(std::initializer_list<value_type> ilist)
		{
			clear();
			t_.insert_unique(ilist.begin(), ilist.end());
			return *this;
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

		void swap(set& rhs) noexcept
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
		friend bool operator==(const set& lhs, const set& rhs) { return lhs.t_ == rhs.t_; }
		friend bool operator< (const set& lhs, const set& rhs) { return lhs.t_ < rhs.t_; }
	};

	//非成员函数
	template< class Key, class Compare, class Alloc >
	bool operator==(const mystl::set<Key, Compare, Alloc>& lhs, const mystl::set<Key, Compare, Alloc>& rhs)
	{
		return lhs == rhs;
	}
	
	template< class Key, class Compare, class Alloc >
	bool operator!=(const mystl::set<Key, Compare, Alloc>& lhs, const mystl::set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator<(const mystl::set<Key, Compare, Alloc>& lhs, const mystl::set<Key, Compare, Alloc>& rhs)
	{
		return lhs < rhs;
	}

	template< class Key, class Compare, class Alloc >
	bool operator<=(const mystl::set<Key, Compare, Alloc>& lhs, const mystl::set<Key, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator>(const mystl::set<Key, Compare, Alloc>& lhs, const mystl::set<Key, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template< class Key, class Compare, class Alloc >
	bool operator>=(const mystl::set<Key, Compare, Alloc>& lhs, const mystl::set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class Key, class Compare, class Alloc >
	void swap(mystl::set<Key, Compare, Alloc>& lhs, mystl::set<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}


	/***************************************************************************
	*				               multiset                                    *
	***************************************************************************/
	//可以存放重复值
	template<typename Key, typename Compare = std::less<Key>, typename Alloc = mystl::allocator<Key>>
	class multiset {
	public:
		using key_type = Key;
		using value_type = Key;
		using key_compare = Compare;
		using value_compare = Compare;
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
		key_compare      key_comp()      const { return t_.key_comp(); }
		value_compare    value_comp()    const { return t_.key_comp(); }
	public:
		//构造函数
		multiset() :t_(Compare()) {}

		explicit multiset(const Compare& comp) :t_(comp) {}

		template< typename InputIt >
		multiset(InputIt first, InputIt last, const Compare& comp = Compare())
			: t_(comp)
		{
			t_.insert_multi(first, last);
		}

		multiset(std::initializer_list<value_type> init, const Compare& comp = Compare())
			:t_(comp)
		{
			t_.insert_multi(init.begin(), init.end());
		}

		multiset(const multiset& other) :t_(other.t_) {}

		multiset(multiset&& other)noexcept :t_(std::move(other.t_)) {}
		//析构
		~multiset() = default;

		//拷贝赋值
		multiset& operator=(const multiset& other)
		{
			t_ = other.t_;
			return *this;
		}

		multiset& operator=(multiset&& other) noexcept
		{
			t_ = std::move(other.t_);
			return *this;
		}

		multiset& operator=(std::initializer_list<value_type> ilist)
		{
			clear();
			t_.insert_multi(ilist.begin(), ilist.end());
			return *this;
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
			return t_.insert_multi(hint, value);
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

		void swap(multiset& rhs) noexcept
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
		friend bool operator==(const multiset& lhs, const multiset& rhs) { return lhs.t_ == rhs.t_; }
		friend bool operator< (const multiset& lhs, const multiset& rhs) { return lhs.t_ < rhs.t_; }
	};

	//非成员函数
	template< class Key, class Compare, class Alloc >
	bool operator==(const mystl::multiset<Key, Compare, Alloc>& lhs, const mystl::multiset<Key, Compare, Alloc>& rhs)
	{
		return lhs == rhs;
	}

	template< class Key, class Compare, class Alloc >
	bool operator!=(const mystl::multiset<Key, Compare, Alloc>& lhs, const mystl::multiset<Key, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator<(const mystl::multiset<Key, Compare, Alloc>& lhs, const mystl::multiset<Key, Compare, Alloc>& rhs)
	{
		return lhs < rhs;
	}

	template< class Key, class Compare, class Alloc >
	bool operator<=(const mystl::multiset<Key, Compare, Alloc>& lhs, const mystl::multiset<Key, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator>(const mystl::multiset<Key, Compare, Alloc>& lhs, const mystl::multiset<Key, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template< class Key, class Compare, class Alloc >
	bool operator>=(const mystl::multiset<Key, Compare, Alloc>& lhs, const mystl::multiset<Key, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class Key, class Compare, class Alloc >
	void swap(mystl::multiset<Key, Compare, Alloc>& lhs, mystl::multiset<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

} //end of namespace mystl

#endif // !_MY_SET_H_
