#pragma once
#ifndef _MY_UNORDERED_SET_H_
#define _MY_UNORDERED_SET_H_
#include"my_hash_table.h"

/*此头文件包含unordered_set和unordered_multiset*/
namespace mystl {
	template<typename Key,typename Hash = mystl::hash<Key>,typename KeyEqual = mystl::equal_to<Key>,typename Alloc = mystl::allocator<Key>>
	class unordered_set {
	public:
		using key_type = Key;
		using value_type = Key;
	private:
		using HashTable = hash_table<Key, Hash, KeyEqual, Alloc>;
		HashTable ht_;
	public:
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using hash_func = Hash;
		using key_equal = KeyEqual;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = typename HashTable::iterator;
		using const_iterator = typename HashTable::const_iterator;

		using local_iterator = typename HashTable::local_iterator;
		using const_local_iterator = typename HashTable::const_local_iterator;

		allocator_type get_allocator() const { return ht_.get_allocator(); }

	public:

		//构造函数
		unordered_set():ht_() {}
		explicit unordered_set(size_type bucket_count,const Hash& hash = Hash(),
			const key_equal& equal = key_equal())
			:ht_(bucket_count,hash,equal){}
		template< typename InputIt >
		unordered_set(InputIt first, InputIt last,
			size_type bucket_count = HashTable::kInitBucketCount,
			const Hash& hash = Hash(), const key_equal& equal = key_equal())
			:ht_(bucket_count,hash,equal)
		{
			ht_.reserve(mystl::distance(first, last));
			ht_.insert_unique(first, last);
		}

		unordered_set(const unordered_set& other) { ht_ = other.ht_; }
		unordered_set(unordered_set&& other)noexcept { ht_ = std::move(ht_); }
		unordered_set(std::initializer_list<value_type> init,
			size_type bucket_count = HashTable::kInitBucketCount,
			const Hash& hash = Hash(),const key_equal& equal = key_equal())
			:unordered_set(init.begin(),init.end(), bucket_count,hash,equal){}
		unordered_set(std::initializer_list<value_type> init,
			size_type bucket_count,const Hash& hash)
			:unordered_set(init.begin(), init.end(), bucket_count, hash){}

		~unordered_set() = default;

		unordered_set& operator=(const unordered_set& other) 
		{ 
			ht_ = other.ht_; 
			return *this;
		}
		unordered_set& operator=(unordered_set&& other) noexcept
		{
			ht_ = std::move(other.ht_);
			return *this;
		}
		unordered_set& operator=(std::initializer_list<value_type> ilist)
		{
			ht_.clear();
			ht_.reserve(ilist.size());
			ht_.insert_unique(ilist.begin(), ilist.end());
			return *this;
		}
	public:
		//迭代器
		iterator begin() noexcept { return ht_.begin(); }
		const_iterator begin() const noexcept { return ht_.begin(); }
		const_iterator cbegin() const noexcept { return ht_.cbegin(); }
		iterator end() noexcept { return ht_.end(); }
		const_iterator end() const noexcept { return ht_.end(); }
		const_iterator cend() const noexcept { return ht_.cend(); }

		//容量
		bool empty() const noexcept { return ht_.empty(); }
		size_type size() const noexcept { return ht_.size(); }
		size_type max_size() const noexcept { return ht_.max_size(); }

		//修改器
		void clear() noexcept {ht_.clear(); }

		std::pair<iterator, bool> insert(const value_type& value) { return ht_.insert_unique(value); }
		std::pair<iterator, bool> insert(value_type&& value) { return ht_.insert_unique(std::move(value)); }
		iterator insert(const_iterator hint, const value_type& value) { return ht_.insert_unique(hint, value); }
		iterator insert(const_iterator hint, value_type&& value) { return ht_.insert_unique(hint, std::move(value)); }
		template< class InputIt >
		void insert(InputIt first, InputIt last) { ht_.insert_unique(first, last); }
		void insert(std::initializer_list<value_type> ilist) { ht_.insert_unique(ilist.begin(), ilist.end()); }

		template< class... Args >
		std::pair<iterator, bool> emplace(Args&&... args) { return ht_.emplace_unique(std::forward<Args>(args)...); }
		template <class... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args) { return ht_.emplace_unique_hint(hint, std::forward<Args>(args)...); }

		iterator erase(const_iterator pos) { return ht_.erase(pos); }
		iterator erase(const_iterator first, const_iterator last) { return ht_.erase(first, last); }
		size_type erase(const key_type& key) { return ht_.erase_unique(key); }

		void swap(unordered_set& other) noexcept { ht_.swap(other.ht_); }

		//查找
		size_type count(const Key& key) const { return ht_.count(key); }

		iterator find(const Key& key) { return ht_.find(key); }
		const_iterator find(const Key& key) const { return ht_.find(key); }

		std::pair<iterator, iterator> equal_range(const Key& key) { return ht_.equal_range(key); }
		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const { return ht_.equal_range(key); }

		//桶接口
		local_iterator begin(size_type n) { return ht_.begin(n); }
		const_local_iterator begin(size_type n) const { return ht_.begin(n); }
		const_local_iterator cbegin(size_type n) const { return ht_.cbegin(n); }
		local_iterator end(size_type n) { return ht_.end(n); }
		const_local_iterator end(size_type n) const { return ht_.end(n); }
		const_local_iterator cend(size_type n) const { return ht_.cend(n); }

		size_type bucket_count() const { return ht_.bucket_count(); }
		size_type max_bucket_count() const { return ht_.max_bucket_count(); }

		size_type bucket_size(size_type n) const { return ht_.bucket_size(n); }
		size_type bucket(const Key& key) const { return ht_.bucket(key); }

		//哈希策略
		float load_factor() const { return ht_.load_factor(); }

		float max_load_factor() const { return ht_.max_load_factor(); }
		void max_load_factor(float ml) { ht_.max_load_factor(ml); } // 设置最大加载因子为 ml (不建议设置)

		void rehash(size_type count) { ht_.rehash(count); }
		void reserve(size_type count) { ht_.reserve(count); }

		//观察器
		hash_func hash_function() const { return ht_.hash_function(); }
		key_equal key_eq() const { return ht_.key_eq(); }

	public:
		friend bool operator==(const unordered_set& lhs, const unordered_set& rhs) { return lhs.ht_ == rhs.ht_; }
	};

	//非成员函数
	template< class Key, class Hash, class KeyEqual, class Alloc >
	bool operator==(const mystl::unordered_set<Key, Hash, KeyEqual, Alloc>& lhs, const mystl::unordered_set<Key, Hash, KeyEqual, Alloc>& rhs)
	{
		return lhs == rhs;
	}

	template< class Key, class Hash, class KeyEqual, class Alloc >
	bool operator!=(const mystl::unordered_set<Key, Hash, KeyEqual, Alloc>& lhs, const mystl::unordered_set<Key, Hash, KeyEqual, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Hash, class KeyEqual, class Alloc >
	void swap(mystl::unordered_set<Key, Hash, KeyEqual, Alloc>& lhs, mystl::unordered_set<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

	/***************************************************************************
	*				          unordered_multiset                               *
	***************************************************************************/

	template<typename Key, typename Hash = mystl::hash<Key>, typename KeyEqual = mystl::equal_to<Key>, typename Alloc = mystl::allocator<Key>>
	class unordered_multiset {
	public:
		using key_type = Key;
		using value_type = Key;
	private:
		using HashTable = hash_table<Key, Hash, KeyEqual, Alloc>;
		HashTable ht_;
	public:
		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using hash_func = Hash;
		using key_equal = KeyEqual;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = typename HashTable::iterator;
		using const_iterator = typename HashTable::const_iterator;

		using local_iterator = typename HashTable::local_iterator;
		using const_local_iterator = typename HashTable::const_local_iterator;

		allocator_type get_allocator() const { return ht_.get_allocator(); }

	public:

		//构造函数
		unordered_multiset() :ht_() {}
		explicit unordered_multiset(size_type bucket_count, const Hash& hash = Hash(),
			const key_equal& equal = key_equal())
			:ht_(bucket_count, hash, equal) {}
		template< typename InputIt >
		unordered_multiset(InputIt first, InputIt last,
			size_type bucket_count = HashTable::kInitBucketCount,
			const Hash& hash = Hash(), const key_equal& equal = key_equal())
			: ht_(bucket_count, hash, equal)
		{
			ht_.reserve(mystl::distance(first, last));
			ht_.insert_multi(first, last);
		}

		unordered_multiset(const unordered_multiset& other) { ht_ = other.ht_; }
		unordered_multiset(unordered_multiset&& other) noexcept{ ht_ = std::move(other.ht_); }
		unordered_multiset(std::initializer_list<value_type> init,
			size_type bucket_count = HashTable::kInitBucketCount,
			const Hash& hash = Hash(), const key_equal& equal = key_equal())
			:unordered_multiset(init.begin(), init.end(), bucket_count, hash, equal){}
		unordered_multiset(std::initializer_list<value_type> init,
			size_type bucket_count, const Hash& hash)
			:unordered_multiset(init.begin(), init.end(), bucket_count, hash){}

		~unordered_multiset() = default;

		unordered_multiset& operator=(const unordered_multiset& other)
		{
			ht_ = other.ht_;
			return *this;
		}
		unordered_multiset& operator=(unordered_multiset&& other) noexcept
		{
			ht_ = std::move(other.ht_);
			return *this;
		}
		unordered_multiset& operator=(std::initializer_list<value_type> ilist)
		{
			ht_.clear();
			ht_.reserve(ilist.size());
			ht_.insert_multi(ilist.begin(), ilist.end());
			return *this;
		}
	public:
		//迭代器
		iterator begin() noexcept { return ht_.begin(); }
		const_iterator begin() const noexcept { return ht_.begin(); }
		const_iterator cbegin() const noexcept { return ht_.cbegin(); }
		iterator end() noexcept { return ht_.end(); }
		const_iterator end() const noexcept { return ht_.end(); }
		const_iterator cend() const noexcept { return ht_.cend(); }

		//容量
		bool empty() const noexcept { return ht_.empty(); }
		size_type size() const noexcept { return ht_.size(); }
		size_type max_size() const noexcept { return ht_.max_size(); }

		//修改器
		void clear() noexcept { ht_.clear(); }

		iterator insert(const value_type& value) { return ht_.insert_multi(value); }
		iterator insert(value_type&& value) { return ht_.insert_multi(std::move(value)); }
		iterator insert(const_iterator hint, const value_type& value) { return ht_.insert_multi(hint, value); }
		iterator insert(const_iterator hint, value_type&& value) { return ht_.insert_multi(hint, std::move(value)); }
		template< class InputIt >
		void insert(InputIt first, InputIt last) { ht_.insert_multi(first, last); }
		void insert(std::initializer_list<value_type> ilist) { ht_.insert_multi(ilist.begin(), ilist.end()); }

		template< class... Args >
		iterator emplace(Args&&... args) { return ht_.emplace_multi(std::forward<Args>(args)...); }
		template <class... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args) { return ht_.emplace_multi_hint(hint, std::forward<Args>(args)...); }

		iterator erase(const_iterator pos) { return ht_.erase(pos); }
		iterator erase(const_iterator first, const_iterator last) { return ht_.erase(first, last); }
		size_type erase(const key_type& key) { return ht_.erase_multi(key); }

		void swap(unordered_multiset& other) noexcept { ht_.swap(other.ht_); }

		//查找
		size_type count(const Key& key) const { return ht_.count(key); }

		iterator find(const Key& key) { return ht_.find(key); }
		const_iterator find(const Key& key) const { return ht_.find(key); }

		std::pair<iterator, iterator> equal_range(const Key& key) { return ht_.equal_range(key); }
		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const { return ht_.equal_range(key); }

		//桶接口
		local_iterator begin(size_type n) { return ht_.begin(n); }
		const_local_iterator begin(size_type n) const { return ht_.begin(n); }
		const_local_iterator cbegin(size_type n) const { return ht_.cbegin(n); }
		local_iterator end(size_type n) { return ht_.end(n); }
		const_local_iterator end(size_type n) const { return ht_.end(n); }
		const_local_iterator cend(size_type n) const { return ht_.cend(n); }

		size_type bucket_count() const { return ht_.bucket_count(); }
		size_type max_bucket_count() const { return ht_.max_bucket_count(); }

		size_type bucket_size(size_type n) const { return ht_.bucket_size(n); }
		size_type bucket(const Key& key) const { return ht_.bucket(key); }

		//哈希策略
		float load_factor() const { return ht_.load_factor(); }

		float max_load_factor() const { return ht_.max_load_factor(); }
		void max_load_factor(float ml) { ht_.max_load_factor(ml); } // 设置最大加载因子为 ml (不建议设置)

		void rehash(size_type count) { ht_.rehash(count); }
		void reserve(size_type count) { ht_.reserve(count); }

		//观察器
		hash_func hash_function() const { return ht_.hash_function(); }
		key_equal key_eq() const { return ht_.key_eq(); }

	public:
		friend bool operator==(const unordered_multiset& lhs, const unordered_multiset& rhs) { return lhs.ht_ == rhs.ht_; }
	};

	//非成员函数
	template< class Key, class Hash, class KeyEqual, class Alloc >
	bool operator==(const mystl::unordered_multiset<Key, Hash, KeyEqual, Alloc>& lhs, const mystl::unordered_multiset<Key, Hash, KeyEqual, Alloc>& rhs)
	{
		return lhs == rhs;
	}

	template< class Key, class Hash, class KeyEqual, class Alloc >
	bool operator!=(const mystl::unordered_multiset<Key, Hash, KeyEqual, Alloc>& lhs, const mystl::unordered_multiset<Key, Hash, KeyEqual, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Hash, class KeyEqual, class Alloc >
	void swap(mystl::unordered_multiset<Key, Hash, KeyEqual, Alloc>& lhs, mystl::unordered_multiset<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

}//end of namespace mystl
#endif // !_MY_UNORDERED_SET_H_
