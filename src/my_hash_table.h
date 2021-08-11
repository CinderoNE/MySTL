#pragma once
#ifndef _MY_HASH_TABLE_H_
#define _MY_HASH_TABLE_H_

#include"my_allocator.h"
#include"my_type_traits.h" //for is_pair_v
#include"my_iterator.h"
#include"my_vector.h"

#include<algorithm>//for std::is_permutation
#include<cmath> // for std::ceil
namespace mystl{

	//非pair类型
	template<typename T, bool = is_pair_v<T>>
	struct hash_table_value_traits {
		using key_type = T;
		using mapped_type = T;
		using value_type = T;

		//set的key和value为同一个
		static const key_type& get_key(const value_type& value)
		{
			return value;
		}


		static const value_type& get_value(const value_type& value)
		{
			return value;
		}
	};

	//pair类型
	template<typename T>
	struct hash_table_value_traits<T,true> {
		using key_type = typename T::first_type;
		using mapped_type = typename T::second_type;
		using value_type = T;

		//map的key为第一个元素，value为pair

		static const key_type& get_key(const value_type& value)
		{
			return value.first;
		}

		static const value_type& get_value(const value_type& value)
		{
			return value;
		}
	};

	template<typename T>
	struct hash_table_node {
		using node_ptr = hash_table_node*;

		node_ptr next;
		T value;
		unsigned hashcode;  //保存计算出的hash值避免重复计算
		hash_table_node() :next(nullptr), value(),hashcode(0) {}
		hash_table_node(const T& val) :next(nullptr), value(val) {}
		hash_table_node(const hash_table_node& other) :next(other.next), value(other.value) {}
		hash_table_node(hash_table_node&& other) :next(other.next), value(std::move(other.value)) {}
	};


	template<typename HashTable>
	struct hash_table_const_iterator :public mystl::iterator<mystl::forward_iterator_tag, typename HashTable::value_type> {
		using value_type = typename HashTable::value_type;
		using pointer = const value_type*;
		using reference = const value_type&;

		using node_ptr = hash_table_node<value_type>*;
		using htb_ptr = HashTable*;
		using self = hash_table_const_iterator;

		

		node_ptr node_p;
		htb_ptr htb_p;  //哈希表的指针，方便迭代器操作

		hash_table_const_iterator() :node_p(nullptr), htb_p(nullptr){}
		hash_table_const_iterator(node_ptr np, htb_ptr hp) :node_p(np), htb_p(hp) {}
		//hash_table_const_iterator(const self& other):node_p(other.node_p), htb_p(other.htb_p) {};
		
		reference operator*() const 
		{
			return node_p->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self& operator++() {
			// 找出当前节点在桶中的下标
			auto idx = node_p->hashcode & (htb_p->bucket_count() - 1);
			node_p = htb_p->get_valid_node(node_p->next, idx);
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		bool operator==(const self& rhs) const { return (node_p == rhs.node_p); }
		bool operator!=(const self& rhs) const { return (node_p != rhs.node_p); }
	};

	template<typename HashTable>
	struct hash_table_iterator :public hash_table_const_iterator<HashTable> {
		using value_type = typename HashTable::value_type;
		using pointer = value_type*;
		using reference = value_type&;

		using node_ptr = hash_table_node<value_type>*;
		using htb_ptr = HashTable*;
		using self = hash_table_iterator;


		using hash_table_const_iterator<HashTable>::node_p;
		using hash_table_const_iterator<HashTable>::htb_p;

		hash_table_iterator() = default;
		hash_table_iterator(node_ptr np, htb_ptr hp) :hash_table_const_iterator<HashTable>(np, hp) {};
		//hash_table_iterator(const self& other):hash_table_const_iterator<HashTable>(other.node_p, other.htb_p) {};

		reference operator*() const
		{
			return node_p->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self& operator++() {
			// 找出当前节点在桶中的下标
			auto idx = node_p->hashcode & (htb_p->bucket_count() - 1);
			node_p = htb_p->get_valid_node(node_p->next, idx);
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}
	};


	//桶迭代器
	template<typename T>
	struct const_bucket_iterator : public mystl::iterator<mystl::forward_iterator_tag, T> {
		using node_ptr = hash_table_node<T>*;
		using self = const_bucket_iterator;

		using value_type = T;
		using reference = const T&;
		using pointer = const T*;

		node_ptr node_p;
		const_bucket_iterator() :node_p(nullptr) {}
		const_bucket_iterator(node_ptr ptr) :node_p(ptr) {};

		reference operator*() const
		{
			return node_p->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self& operator++() {
			ASSERT_EXPR(node_p != nullptr);
			node_p = node_p->next;
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}
		bool operator==(const self& rhs) const { return (node_p == rhs.node_p); }
		bool operator!=(const self& rhs) const { return (node_p != rhs.node_p); }
	};

	template<typename T>
	struct bucket_iterator : public const_bucket_iterator<T> {
		using node_ptr = hash_table_node<T>*;
		using self = bucket_iterator;

		using value_type = T;
		using reference = T&;
		using pointer = T*;

		using const_bucket_iterator<T>::node_p;

		bucket_iterator() = default;
		bucket_iterator(node_ptr ptr) :const_bucket_iterator<T>(ptr) {};

		reference operator*() const
		{
			return node_p->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self& operator++() {
			ASSERT_EXPR(node_p != nullptr);
			node_p = node_p->next;
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}
	};


	template<typename T, typename Hash, typename  KeyEqual, typename Alloc = mystl::allocator<T>>
	class hash_table {
	public:
		//T不为pair时，key_type,mapped_type,value_type相同
		//T为pair时，key_type为pair的第一个元素,mapped_type为第二个元素,value_type为pair
		using value_traits = hash_table_value_traits<T>;

		using key_type = typename value_traits::key_type;
		using mapped_type = typename value_traits::mapped_type;
		using value_type = typename value_traits::value_type;
		using hash_func = Hash;
		using key_equal = KeyEqual;

		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = hash_table_iterator<hash_table>;
		using const_iterator = hash_table_const_iterator<hash_table>;

		using local_iterator = bucket_iterator<value_type>;
		using const_local_iterator = const_bucket_iterator<value_type>;

		using node_type = hash_table_node<value_type>;
		using node_ptr = node_type*;

		using node_allocator = mystl::allocator<node_type>;
		using data_allocator = Alloc;

		allocator_type get_allocator() const { return data_allocator(); }

		static constexpr size_t kInitBucketCount = 16;  //默认初始桶大小
		static constexpr float kInitMaxLoadFactor = 0.75; //默认加载因子
		static constexpr size_type kMaxBucketCount = 1 << 30;//最大桶数量

		//便于使用hashtable中的方法
		friend struct hash_table_iterator<hash_table>;
		friend struct hash_table_const_iterator<hash_table>;
		
	private:
		size_type bucket_count_; //桶数量
		vector<node_ptr> buckets_; //桶
		size_type node_count_;  //元素数量
		hash_func hash_;   //hash函数
		key_equal key_equal_; //key比较函数
		float max_load_factor_; //加载因子（当node_count_ / buckets_.size() >= load_factor_ 时就要扩容）

	private:
		void copy_buckets(const vector<node_ptr>& other_buckets);

		//https://www.zhihu.com/question/422840340
		//高16位与低16位异或，扰乱哈希值，增加随机性
		int hash_disturb(unsigned hashcode) const { return hashcode ^ ((hashcode) >> 16); }
		//先执行hash函数，然后获得hash值对应的桶下标（&操作的前提是桶的数量是2的倍数）
		size_type bucket_index(const key_type& key) const { return hash_disturb(hash_(key)) & (bucket_count_ - 1); }
		const key_type& node_key(node_ptr ptr) const { return value_traits::get_key(ptr->value); } //获取节点的key

		template<typename ...Args>
		node_ptr create_node(Args... args);
		void destroy_node(node_ptr ptr);

		void rehash() {
			//每次扩容两倍
			rehash(bucket_count_ * 2);
		}
		//找到一个有效节点,如果node_count_为0，直接返回nullptr
		// 如果ptr不为nullptr，则返回ptr
		// 否则寻找下一个有效节点
		//start_idx :ptr所在桶的下标
		node_ptr get_valid_node(node_ptr ptr, size_type ptr_idx) const;

	public:
		explicit hash_table(size_type bucket_count = kInitBucketCount, const hash_func& hash = hash_func(),
			const key_equal key_eq = key_equal(), float max_load_factor = kInitMaxLoadFactor)
			:bucket_count_(ge_near_pow2(bucket_count)),
			buckets_(bucket_count, nullptr),
			node_count_(0),
			hash_(hash),
			key_equal_(key_eq),
			max_load_factor_(max_load_factor)
		{
		}

		hash_table(const hash_table& other);
			
		hash_table(hash_table&& other) noexcept
			:bucket_count_(other.bucket_count_), 
			buckets_(std::move(other.buckets_)),
			node_count_(other.node_count_),
			hash_(other.hash_),
			key_equal_(other, key_equal_),
			max_load_factor_(other.max_load_factor_)
		{
			other.bucket_count_ = 0;
			other.node_count_ = 0;
		}

		hash_table& operator=(const hash_table& other);
		hash_table& operator=(hash_table&& other) noexcept
		{
			if (this != &other) {
				clear();
				bucket_count_ = other.bucket_count_;
				buckets_ = std::move(other.buckets_);
				node_count_ = other.node_count_;
				hash_ = other.hash_;
				key_equal_ = other.key_equal_;
				max_load_factor_ = other.max_load_factor_;
				other.bucket_count_ = 0;
				other.node_count_ = 0;
			}
			return *this;
		}

		~hash_table() {
			clear();
		}

	public:
		//迭代器
		iterator begin()noexcept 
		{ 
			iterator ret(nullptr, this);
			for (size_type i = 0; i < bucket_count_; ++i) {
				if (buckets_[i] != nullptr) {
					ret.node_p = buckets_[i];
					break;
				}
			}
			return ret;
		}
		const_iterator begin()const noexcept 
		{  
			const_iterator ret(nullptr, const_cast<hash_table*>(this));
			for (size_type i = 0; i < bucket_count_; ++i) {
				if (buckets_[i] != nullptr) {
					ret.node_p = buckets_[i];
					break;
				}
			}
			return ret;
		}
		const_iterator cbegin()const noexcept { return begin(); }
		iterator end()noexcept { return iterator(nullptr,this); };
		const_iterator end()const noexcept { return const_iterator(nullptr,const_cast<hash_table*>(this)); }
		const_iterator cend()const noexcept { return end(); }

		//容量
		bool      empty() const noexcept { return node_count_ == 0; }
		size_type size() const noexcept { return node_count_; }
		size_type max_size() const noexcept { return static_cast<size_type>(kMaxBucketCount * kInitMaxLoadFactor); }

	public:
		//修改器
		void clear();

		//insert
		std::pair<iterator, bool> insert_unique(const value_type& value);
		std::pair<iterator, bool> insert_unique(value_type&& value);
		//以 hint 为开始搜索的位置。返回指向被插入元素，或阻止插入的元素的迭代器。
		iterator insert_unique(const_iterator hint, const value_type& value);
		iterator insert_unique(const_iterator hint, value_type&& value);
		template< typename InputIt , std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		void insert_unique(InputIt first, InputIt last);
		

		iterator insert_multi(const value_type& value);
		iterator insert_multi(value_type&& value);
		//以 hint 为开始搜索的位置。返回指向新插入元素的迭代器。
		iterator insert_multi(const_iterator hint, const value_type& value);
		iterator insert_multi(const_iterator hint, value_type&& value);
		template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		void insert_multi(InputIt first, InputIt last);

		//emplace
		template <typename ...Args>
		std::pair<iterator, bool> emplace_unique(Args&& ...args);
		template <typename ...Args>
		iterator emplace_unique_hint(const_iterator hint, Args&& ...args);

		template <typename ...Args>
		iterator emplace_multi(Args&& ...args);
		template <typename ...Args>
		iterator emplace_multi_hint(const_iterator hint, Args&& ...args);

		//erase
		iterator erase(const_iterator pos);

		size_type erase_multi(const key_type& key);
		size_type erase_unique(const key_type& key);

		iterator erase(const_iterator first, const_iterator last);

		void swap(hash_table& other) noexcept;
	public:
		//操作
		size_type count(const key_type& key) const;

		iterator find(const key_type& key);
		const_iterator find(const key_type& key) const;

		std::pair<iterator, iterator> equal_range(const key_type& key);
		std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

	public:
		//桶接口
		local_iterator begin(size_type n) 
		{ 
			ASSERT_EXPR(n < bucket_count_);
			return buckets_[n]; 
		}
		const_local_iterator begin(size_type n) const
		{
			ASSERT_EXPR(n < bucket_count_);
			return buckets_[n];
		}
		const_local_iterator cbegin(size_type n) const
		{
			ASSERT_EXPR(n < bucket_count_);
			return buckets_[n];
		}

		local_iterator end(size_type n)
		{
			ASSERT_EXPR(n < bucket_count_);
			return nullptr;
		}
		const_local_iterator end(size_type n) const
		{
			ASSERT_EXPR(n < bucket_count_);
			return nullptr;
		}
		const_local_iterator cend(size_type n) const
		{
			ASSERT_EXPR(n < bucket_count_);
			return nullptr;
		}

		size_type bucket_count() const { return bucket_count_; }
		constexpr size_type max_bucket_count() const { return kMaxBucketCount; }
		//返回下标为 n 的桶中的元素数。
		size_type bucket_size(size_type n) const;
		//返回关键 key 的桶的下标
		size_type bucket(const key_type& key) const { return bucket_index(key); }

		//哈希策略
 
		//返回每个桶元素的平均数。
		float load_factor() const { return static_cast<float>(node_count_) / bucket_count_; }

		//管理最大加载因子（每个桶的平均元素数）。若加载因子超出此阈值，则容器自动增加桶数。
		// 返回最大加载因子
		float max_load_factor() const { return max_load_factor_; }
		// 设置最大加载因子为 ml (不建议设置)
		void max_load_factor(float ml) { max_load_factor_ = ml; }

		//设置桶数为 count 并重哈希容器，若新的桶数使加载因子大于最大加载因子（ count < size() / max_load_factor() ）
		//新桶数至少为ge_near_pow2(size() / max_load_factor())
		void rehash(size_type count);

		//设置桶数为适应至少 count 个元素，而不超出最大加载因子所需的数，并重哈希容器。
		void reserve(size_type count)
		{
			size_type suggest_bucket_count = ge_near_pow2(static_cast<size_type>(std::ceil(count / max_load_factor())));
			rehash(suggest_bucket_count);
		}

		//观察器
		hash_func hash_function() const { return hash_; }
		key_equal key_eq() const { return key_equal_; }

	private:
		//最接近n的大于等于n的2的倍数
		size_type ge_near_pow2(size_type n) const {
			if ((n & (n - 1)) == 0)
				return n;
			while ((n & (n - 1)) != 0) {
				n = n & (n - 1);
			}
			return n << 1;
		}	
	};

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::copy_buckets(const vector<node_ptr>& other_buckets)
	{
		//逐个复制节点
		for (size_type i = 0; i < bucket_count_; ++i) {
			node_ptr curr = other_buckets[i];
			if (curr != nullptr) {
				node_ptr new_node = create_node(curr->value);
				buckets_[i] = new_node;
				while (curr->next != nullptr) {
					new_node->next = create_node(curr->next->value);
					curr = curr->next;
					new_node = new_node->next;
				}
			}
		}
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename ...Args>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::node_ptr 
		hash_table<T, Hash, KeyEqual, Alloc>::create_node(Args ...args)
	{
		node_ptr new_node = node_allocator::allocate(1);
		mystl::construct(std::addressof(new_node->value), std::forward<Args>(args)...);
		new_node->next = nullptr;
		new_node->hashcode = hash_disturb(hash_(node_key(new_node)));  //创建节点时就计算出哈希值
		return new_node;
	}


	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::destroy_node(node_ptr ptr)
	{
		mystl::destroy_at(std::addressof(ptr->value));
		node_allocator::deallocate(ptr, 1);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	typename hash_table<T, Hash, KeyEqual, Alloc>::node_ptr 
		hash_table<T, Hash, KeyEqual, Alloc>::get_valid_node(node_ptr ptr, size_type ptr_idx) const
	{
		if (node_count_ == 0) {
			return nullptr;
		}
		if (ptr != nullptr) {
			return ptr;
		}
		else {
			//在之后的桶中寻找第一个有效的节点
			while (++ptr_idx < bucket_count_) {
				if (buckets_[ptr_idx] != nullptr) {
					return buckets_[ptr_idx];
				}
			}
			return nullptr;
		}
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline hash_table<T, Hash, KeyEqual, Alloc>::hash_table(const hash_table& other)
		:bucket_count_(other.bucket_count_), 
		buckets_(other.bucket_count_, nullptr),
		node_count_(other.node_count_),
		hash_(other.hash_),
		key_equal_(other.key_equal_),
		max_load_factor_(other.max_load_factor_)
	{
		copy_buckets(other.buckets_);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline hash_table<T, Hash, KeyEqual, Alloc>& hash_table<T, Hash, KeyEqual, Alloc>::operator=(const hash_table& other)
	{
		if (this != &other) {
			clear();
			bucket_count_ = other.bucket_count_;
			buckets_.resize(bucket_count_, nullptr);
			node_count_ = other.node_count_;
			hash_ = other.hash_;
			key_equal_ = other.key_equal_;
			max_load_factor_ = other.max_load_factor_;
			copy_buckets(other.buckets_);
		}
		return *this;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::clear()
	{
		if (node_count_ > 0) {
			for (size_type i = 0; i < bucket_count_; ++i) {
				node_ptr curr = buckets_[i];
				while (curr != nullptr) {
					node_ptr next = curr->next;
					destroy_node(curr);
					curr = next;
				}
				buckets_[i] = nullptr;
			}
			node_count_ = 0;
		}
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	std::pair<typename hash_table<T, Hash, KeyEqual, Alloc>::iterator, bool> 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_unique(const value_type& value)
	{
		const key_type& key = value_traits::get_key(value);
		size_type idx = bucket_index(key);

		node_ptr new_node = nullptr;//保证在需要的时候才创建
		if (buckets_[idx] == nullptr) {
			//选择的桶为空，一定没有相同值
			buckets_[idx] = new_node = create_node(value);
		}
		else {
			node_ptr curr = buckets_[idx];
			if (key_equal_(key, node_key(curr))) {
				//与桶的第一个节点相同，返回key相同节点的迭代器
				return std::make_pair(iterator(curr, this), false);
			}
			while (curr->next != nullptr) {
				if (key_equal_(key,node_key(curr))) {
					//存在相同的key，返回key相同节点的迭代器
					return std::make_pair(iterator(curr, this),false);
				}
				curr = curr->next;
			}
			curr->next = new_node = create_node(value);
		}

		if (++node_count_ > bucket_count_ * max_load_factor_) {
			rehash();
		}
		//此时new_node肯定不为nullptr
		return std::make_pair(iterator(new_node, this), true);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	std::pair<typename hash_table<T, Hash, KeyEqual, Alloc>::iterator, bool> 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_unique(value_type&& value)
	{
		const key_type& key = value_traits::get_key(value);
		size_type idx = bucket_index(key);

		node_ptr new_node = nullptr;//保证在需要的时候才创建
		if (buckets_[idx] == nullptr) {
			//选择的桶为空，一定没有相同值
			buckets_[idx] = new_node = create_node(std::move(value));
		}
		else {
			node_ptr curr = buckets_[idx];
			if(key_equal_(key,node_key(curr))) {
				//与桶的第一个节点相同，返回key相同节点的迭代器
				return std::make_pair(iterator(curr, this), false);
			}
			while (curr->next != nullptr) {
				if (key_equal_(key, node_key(curr))) {
					//存在相同的key，返回key相同节点的迭代器
					return std::make_pair(iterator(curr, this), false);
				}
				curr = curr->next;
			}
			curr->next = new_node = create_node(std::move(value));
		}

		if (++node_count_ > bucket_count_ * max_load_factor_) {
			rehash();
		}
		//此时new_node肯定不为nullptr
		return std::make_pair(iterator(new_node, this), true);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_unique(const_iterator hint, const value_type& value)
	{
		//在这里hint用处不大，因为要确定没有重复值，只能从桶的第一个节点开始比较
		(void)hint;
		return insert_unique(value).first;

	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_unique(const_iterator hint, value_type&& value)
	{
		//在这里hint用处不大，因为要确定没有重复值，只能从桶的第一个节点开始比较
		(void)hint;
		return insert_unique(std::move(value)).first;
	}


	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::insert_unique(InputIt first, InputIt last)
	{
		while (first != last) {
			insert_unique(*first);
			++first;
		}
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_multi(const value_type& value)
	{
		return emplace_multi(value);

	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_multi(value_type&& value)
	{
		return emplace_multi(std::move(value));
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_multi(const_iterator hint, const value_type& value)
	{
		return emplace_multi_hint(hint, value);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::insert_multi(const_iterator hint, value_type&& value)
	{
		return emplace_multi_hint(hint, std::move(value));
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::insert_multi(InputIt first, InputIt last)
	{
		while (first != last) {
			emplace_multi(*first);
			++first;
		}
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename ...Args>
	std::pair<typename hash_table<T, Hash, KeyEqual, Alloc>::iterator, bool> 
		hash_table<T, Hash, KeyEqual, Alloc>::emplace_unique(Args && ...args)
	{
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);
		size_type idx = new_node->hashcode & (bucket_count_ - 1);

		
		if (buckets_[idx] == nullptr) {
			//选择的桶为空，一定没有相同值
			buckets_[idx] = new_node;
		}
		else {
			node_ptr curr = buckets_[idx];
			if (key_equal_(key, node_key(curr))) {
				destroy_node(new_node);  //要删除创建出的节点
				//与桶的第一个节点相同，返回key相同节点的迭代器
				return std::make_pair(iterator(curr, this), false);
			}
			while (curr->next != nullptr) {
				if (key_equal_(key, node_key(curr))) {
					destroy_node(new_node);//要删除创建出的节点
					//存在相同的key，返回key相同节点的迭代器
					return std::make_pair(iterator(curr, this), false);
				}
				curr = curr->next;
			}
			curr->next = new_node;
		}

		if (++node_count_ > bucket_count_ * max_load_factor_) {
			rehash();
		}
		return std::make_pair(iterator(new_node, this), true);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename ...Args>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::emplace_unique_hint(const_iterator hint, Args && ...args)
	{
		//在这里hint用处不大，因为要确定没有重复值，只能从桶的第一个节点开始比较
		(void)hint;
		return emplace_unique(std::forward<Args>(args)...).first;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename ...Args>
	typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::emplace_multi(Args && ...args)
	{
		//一定会插入，所以先检查要不要扩容，免得插入之后再扩容，又hash一次
		if (++node_count_ > bucket_count_ * max_load_factor_) {
			rehash();
		}
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);
		size_type idx = new_node->hashcode & (bucket_count_ - 1);

		if (buckets_[idx] == nullptr) {
			buckets_[idx] = new_node;
		}
		else {
			node_ptr curr = buckets_[idx];
			//如果存在相同的key，就插入到它后面，保证相同的key都在一起
			while (curr != nullptr) {
				if (key_equal_(key, node_key(curr))) {
					new_node->next = curr->next;
					curr->next = new_node;
					break;
				}
				curr = curr->next;
			}
			//不存在则在头部插入
			if (curr == nullptr) {
				new_node->next = buckets_[idx];
				buckets_[idx] = new_node;
			}
		}
		return iterator(new_node, this);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	template<typename ...Args>
	typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::emplace_multi_hint(const_iterator hint, Args && ...args)
	{

		node_ptr hint_node = hint.node_p;
		//hint == end()
		if (hint_node == nullptr) {
			return emplace_multi(std::forward<Args>(args)...);
		}

		//一定会插入，所以先检查要不要扩容，免得插入之后再扩容，又hash一次
		if (++node_count_ > bucket_count_ * max_load_factor_) {
			rehash();
		}
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);
		size_type idx = new_node->hashcode & (bucket_count_ - 1);

		const key_type& hit_key = node_key(hint_node);

		//与hint的key相同，直接在hint_node后插入
		if (key_equal_(key, hit_key)) {
			new_node->next = hint_node->next;
			hint_node->next = new_node;
			return iterator(new_node, this);
		}
		else {
			//从桶的第一个节点寻找插入点
			//如果存在相同的key，就插入到它后面，保证相同的key都在一起
			//不存在则在尾部插入
			node_ptr curr = buckets_[idx];
			if (curr == nullptr) {
				buckets_[idx] = new_node;
			}
			else {
				//如果存在相同的key，就插入到它后面，保证相同的key都在一起
				while (curr != nullptr) {
					if (key_equal_(key, node_key(curr))) {
						new_node->next = curr->next;
						curr->next = new_node;
						break;
					}
					curr = curr->next;
				}
				//不存在则在头部插入
				if (curr == nullptr) {
					new_node->next = buckets_[idx];
					buckets_[idx] = new_node;
				}
			}
		}

		return iterator(new_node, this);
	}


	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::erase(const_iterator pos)
	{
		node_ptr ptr = pos.node_p;
		size_type idx = ptr->hashcode & (bucket_count_ - 1);

		node_ptr curr = buckets_[idx];
		iterator ret(nullptr, this);
		if (curr == ptr) {
			//桶的第一个元素就是目标节点
			buckets_[idx] = curr->next;
			destroy_node(curr);
			--node_count_;
			//找到下一个有效节点
			ret.node_p = get_valid_node(buckets_[idx], idx);
		}
		else {
			node_ptr prev = curr;
			curr = curr->next;
			while (curr != nullptr) {
				if (curr == ptr) {
					//连接前一个节点和后一个节点
					prev->next = curr->next;
					destroy_node(curr);
					--node_count_;
					//找到下一个有效节点
					ret.node_p = get_valid_node(prev->next, idx);
					break;
				}
				prev = curr;
				curr = curr->next;
			}
		}
		return ret;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::size_type 
		hash_table<T, Hash, KeyEqual, Alloc>::erase_multi(const key_type& key)
	{
		auto p = equal_range(key);
		if (p.first.node_p != nullptr) {
			erase(p.first, p.second);
			return mystl::distance(p.first, p.second);
		}
		return 0;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	typename hash_table<T, Hash, KeyEqual, Alloc>::size_type 
		hash_table<T, Hash, KeyEqual, Alloc>::erase_unique(const key_type& key)
	{
		size_type idx = bucket_index(key);

		node_ptr curr = buckets_[idx];
		if (curr == nullptr) {
			return 0;
		}
		else {
			if (key_equal_(key, node_key(curr))) {
				//删除桶的第一个节点
				buckets_[idx] = curr->next;
				destroy_node(curr);
				--node_count_;
				return 1;
			}
			node_ptr prev = curr;
			curr = curr->next;
			while (curr != nullptr) {
				if (key_equal_(key, node_key(curr))) {
					//删除链表上第一个与key相等的节点
					prev->next = curr->next;
					destroy_node(curr);
					--node_count_;
					return 1;
				}
				prev = curr;
				curr = curr->next;
			}
		}
		return 0;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::erase(const_iterator first, const_iterator last)
	{
		if (first.node_p == last.node_p) {
			return end();
		}
		node_ptr first_ptr = first.node_p;
		node_ptr last_ptr = last.node_p;

		size_type first_bucket_idx = first_ptr->hashcode & (bucket_count_ - 1);
		size_type last_bucket_idx = last_ptr->hashcode & (bucket_count_ - 1);

		//在同一个桶中删除
		if (first_bucket_idx == last_bucket_idx) {
			//首先将first_ptr前一个节点与last_ptr连接
			//first_ptr是桶中的第一个元素
			if (first_ptr == buckets_[first_bucket_idx]) {
				buckets_[first_bucket_idx] = last_ptr;
			}
			else {
				//找到first_ptr的前一个节点
				node_ptr first_prev = buckets_[first_bucket_idx];
				while (first_prev->next != first_ptr) {
					first_prev = first_prev->next;
				}
				first_prev->next = last_ptr;
			}
			while (first_ptr != last_ptr) {
				node_ptr next = first_ptr->next;
				destroy_node(first_ptr);
				--node_count_;
				first_ptr = next;
			}
		}
		else {
			//在不同的桶中
			//首先将first_ptr前一个节点的next指向nullptr
			if (first_ptr == buckets_[first_bucket_idx]) {
				//first_ptr是桶中的第一个元素
				buckets_[first_bucket_idx] = nullptr;
			}
			else {
				//找到first_ptr的前一个节点
				node_ptr first_prev = buckets_[first_bucket_idx];
				while (first_prev->next != first_ptr) {
					first_prev = first_prev->next;
				}
				first_prev->next = nullptr;
			}
			while (first_ptr != nullptr) {
				node_ptr next = first_ptr->next;
				destroy_node(first_ptr);
				--node_count_;
				first_ptr = next;
			}
			//删除之后桶的节点
			for (++first_bucket_idx; first_bucket_idx != last_bucket_idx; ++first_bucket_idx) {
				node_ptr curr = buckets_[first_bucket_idx];
				while (curr != nullptr) {
					node_ptr next = curr->next;
					destroy_node(curr);
					--node_count_;
					curr = next;
				}
				buckets_[first_bucket_idx] = nullptr;
			}
			//last_bucket_idx 是有效下标
			if (first_bucket_idx != bucket_count_) {
				node_ptr curr = buckets_[first_bucket_idx];
				while (curr != last_ptr) {
					node_ptr next = curr->next;
					destroy_node(curr);
					--node_count_;
					curr = next;
				}
				buckets_[first_bucket_idx] = last_ptr;
			}
		}

		return iterator(last_ptr, this);

	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::swap(hash_table& other) noexcept
	{
		if (this != &other) {
			mystl::swap(bucket_count_, other.bucket_count_);
			mystl::swap(buckets_, other.buckets_);
			mystl::swap(node_count_, other.node_count_);
			mystl::swap(hash_, other.hash_);
			mystl::swap(key_equal_, other.key_equal_);
			mystl::swap(max_load_factor_, other.max_load_factor_);
		}
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::size_type 
		hash_table<T, Hash, KeyEqual, Alloc>::count(const key_type& key) const
	{
		size_type idx = bucket_index(key);
		size_type ret = 0;
		
		node_ptr curr = buckets_[idx];
		while (curr != nullptr) {
			if (key_equal_(key, node_key(curr))) {
				++ret;
			}
			curr = curr->next;
		}
		return ret;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::find(const key_type& key)
	{
		//调用const版本的find
		const_iterator it = static_cast<const hash_table&>(*this).find(key);
		return iterator(it.node_p, this);
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::const_iterator 
		hash_table<T, Hash, KeyEqual, Alloc>::find(const key_type& key) const
	{
		size_type idx = bucket_index(key);

		const_iterator ret(nullptr, const_cast<hash_table*>(this));
		node_ptr curr = buckets_[idx];
		while (curr != nullptr) {
			if (key_equal_(key, node_key(curr))) {
				ret.node_p = curr;
			}
			curr = curr->next;
		}
		return ret;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline std::pair<typename hash_table<T, Hash, KeyEqual, Alloc>::iterator, typename hash_table<T, Hash, KeyEqual, Alloc>::iterator> 
		hash_table<T, Hash, KeyEqual, Alloc>::equal_range(const key_type& key)
	{
		//调用const版本的equal_range
		auto p = static_cast<const hash_table&>(*this).equal_range(key);
		return std::make_pair(iterator(p.first.node_p, this), iterator(p.second.node_p, this));
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	std::pair<typename hash_table<T, Hash, KeyEqual, Alloc>::const_iterator, typename hash_table<T, Hash, KeyEqual, Alloc>::const_iterator> 
		hash_table<T, Hash, KeyEqual, Alloc>::equal_range(const key_type& key) const
	{
		size_type idx = bucket_index(key);
		const_iterator first(nullptr, const_cast<hash_table*>(this));

		node_ptr curr = buckets_[idx];
		while (curr != nullptr) {
			if (key_equal_(key, node_key(curr))) {
				//第一个与key相等的元素赋值给first
				if (first.node_p == nullptr) {
					first.node_p = curr;
				}
			}
			else if(first.node_p != nullptr){
				//first赋值过，说明之前找到过key相等的节点
				break;
			}
			curr = curr->next;
		}
		
		return std::make_pair(first, const_iterator(get_valid_node(curr, idx), const_cast<hash_table*>(this)));
	}


	/*===============桶接口===============*/
	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline typename hash_table<T, Hash, KeyEqual, Alloc>::size_type 
		hash_table<T, Hash, KeyEqual, Alloc>::bucket_size(size_type n) const
	{
		ASSERT_EXPR(n < bucket_count_);
		node_ptr curr = buckets_[n];
		size_type ret = 0;
		while (curr != nullptr) {
			++ret;
			curr = curr->next;
		}
		return ret;
	}

	template<typename T, typename Hash, typename KeyEqual, typename Alloc>
	inline void hash_table<T, Hash, KeyEqual, Alloc>::rehash(size_type count)
	{
		//至少需要多少桶
		size_type least_bucket_count = static_cast<size_type>(std::ceil(node_count_ / max_load_factor_));
		if (count < least_bucket_count) {
			count = ge_near_pow2(least_bucket_count);
		}
		else {
			count = ge_near_pow2(count);
		}

		vector<node_ptr> new_buckets(count, nullptr);

		if (!empty()) {
			//TODO java1.8中拆分链表法（感觉没性能提升啊。。）
			for (size_type i = 0; i < bucket_count_; ++i) {
				node_ptr curr = buckets_[i];

				while (curr != nullptr) {
					node_ptr next = curr->next;
					size_type idx = curr->hashcode & (count - 1);

					//插入到新桶的头部
					curr->next = new_buckets[idx];
					new_buckets[idx] = curr;

					curr = next;
				}
			}
		}
		mystl::swap(buckets_, new_buckets);
		bucket_count_ = count;
	}




	//非成员函数
	template <typename T, typename Hash, typename  KeyEqual, typename Alloc>
	bool operator==(const hash_table<T, Hash, KeyEqual, Alloc>& lhs, const hash_table<T, Hash, KeyEqual, Alloc>& rhs)
	{
		using value_traits = typename hash_table<T, Hash, KeyEqual, Alloc>::value_tratis;
		if (lhs.size() != rhs.size()) {
			return false;
		}
		else if(lhs.size() != 0){//节点数相同且不为0
			//获取第一个有效节点
			auto curr = lhs.begin().node_p;
			while (curr != nullptr) {
				auto& key = value_traits::get_key(curr->value);
				auto p1 = lhs.equal_range(key);
				auto p2 = rhs.equal_range(key);
				if (mystl::distance(p1.first, p1.second) != mystl::distance(p2.first, p2.second)) {
					return false;
				}
				//p2不是p1的排列
				if (!std::is_permutation(p1.first, p1.second, p2.first)) {
					return false;
				}
				curr = p1.second;
			}
		}
		return true;
	}
	template <typename T, typename Hash, typename  KeyEqual, typename Alloc>
	bool operator!=(const hash_table<T, Hash, KeyEqual, Alloc>& lhs, const hash_table<T, Hash, KeyEqual, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}


	template <typename T, typename Hash, typename  KeyEqual, typename Alloc>
	void swap(const hash_table<T, Hash, KeyEqual, Alloc>& lhs, const hash_table<T, Hash, KeyEqual, Alloc>& rhs) noexcept {
		lhs.swap(rhs);
	}

	

}

#endif // !_MY_HASH_TABLE_H_
