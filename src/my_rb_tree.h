#pragma once
#ifndef _MY_RB_TREE_H_
#define _MY_RB_TREE_H_

#include"my_iterator.h"
#include"my_allocator.h"
#include"my_algorithm.h"
#include"my_type_traits.h" //for is_pair_v

#include<utility> //for pair
#include<iostream>
namespace mystl {

	using rb_tree_node_color = bool;

	static constexpr rb_tree_node_color Red = false;

	static constexpr rb_tree_node_color Black = true;

	//非pair类型
	template<typename T, bool = is_pair_v<T>>
	struct rb_tree_value_traits {
		using key_type = T;
		using mapped_type = T;//set 没有mapped_type
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
	struct rb_tree_value_traits<T, true> {
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


	struct rb_tree_node_base {
		using base_ptr = rb_tree_node_base*;

		rb_tree_node_color color = Red;
		base_ptr parent = nullptr;
		base_ptr left = nullptr;
		base_ptr right = nullptr;

		rb_tree_node_base()
			:color(Red), parent(nullptr), left(nullptr), right(nullptr) {}

		//后继节点
		base_ptr successor() {
			if (right != nullptr) {
				//右子树的最小节点
				return min_node(right);
			}
			else {
				//只有在this == root,且root为max_node时发生
				//因为红黑树的实现中，header 与 root互为父节点
				if (parent->parent == this) {
					return parent;
				}
				//没有右节点
				//如果当前节点为左孩子，则后继节点为父节点
				//如果当前节点为右孩子，则找第一个为左孩子的节点,它的父节点就是后继节点
				base_ptr ret = parent;
				base_ptr curr = this;
				while (curr == ret->right) {
					curr = ret;
					ret = ret->parent;
				}
				return ret;

			}
		}

		//前驱节点
		base_ptr precursor() {
			//this == header,header为红黑树的end(),end()的前驱就是最大的节点
			if (parent->parent == this && color == Red) {
				return right;
			}
			if (left != nullptr) {
				//左子树的最大节点
				return max_node(right);
			}
			else {
				//没有左子树
				//如果当前节点为右孩子，则前驱节点为父节点
				//如果当前节点为左孩子，则找第一个为右孩子的节点,它的父节点就是前驱节点
				base_ptr ret = parent;
				base_ptr curr = this;
				while (curr == ret->left) {
					curr = ret;
					ret = ret->parent;
				}
				return ret;
			}
		}

		//祖父节点
		base_ptr grandparent() {
			if (parent == nullptr) {
				return nullptr;
			}
			return parent->parent;
		}

		//叔父节点
		base_ptr uncle() {
			if (parent == nullptr) {
				return nullptr;
			}
			if (parent == grandparent()->left) {
				return grandparent()->right;
			}
			else {
				return grandparent()->left;
			}
		}

		//兄弟节点
		base_ptr brother() {
			if (parent->left == this) {
				return parent->right;
			}
			return parent->left;
		}

		//权值最小的节点,也就是最左的节点
		static base_ptr min_node(base_ptr ptr) {
			while (ptr->left != nullptr) {
				ptr = ptr->left;
			}
			return ptr;
		}

		//权值最大的节点,也就是最右的节点
		static base_ptr max_node(base_ptr ptr) {
			while (ptr->right != nullptr) {
				ptr = ptr->right;
			}
			return ptr;
		}

		//节点操作
		//右旋p，旋转后p成为新的父节点
			//p为父节点的左节点
		static void rotate_right(base_ptr p, base_ptr& root)
		{
			base_ptr father = p->parent;

			//当前节点的右子树接到父节点的左子树
			father->left = p->right;
			if (p->right != nullptr) {
				p->right->parent = father;
			}

			p->parent = father->parent; //当前节点的父节点重设为祖父节点

			//父节点为root
			if (father == root) {
				//将header节点的parent设置为新的root节点
				root->parent->parent = p;
				root = p;
			}
			else if (father == father->parent->left) { //父节点为左子树
				father->parent->left = p;
			}
			else {  //父节点为右子树
				father->parent->right = p;
			}
			//原父节点成为p的右子树
			p->right = father;
			father->parent = p;

		}

		//左旋p，旋转后p成为新的父节点
		static void rotate_left(base_ptr p, base_ptr& root)
		{
			base_ptr father = p->parent;

			//当前节点的左子树接到父节点的右子树
			father->right = p->left;
			if (p->left != nullptr) {
				p->left->parent = father;
			}

			p->parent = father->parent; //当前节点的父节点重设为祖父节点

			//父节点为root
			if (father == root) {
				//将header节点的parent设置为新的root节点
				root->parent->parent = p;
				root = p;
			}
			else if (father == father->parent->left) { //父节点为左子树
				father->parent->left = p;
			}
			else {  //父节点为右子树
				father->parent->right = p;
			}
			//原父节点成为p的左子树
			p->left = father;
			father->parent = p;
		}

		//使红黑树重新平衡
		//ptr:新插入的节点
		//root:根节点
		// 
		// Tips:新增节点最开始都为红色
		// 
		// insert_case1:新增节点是根节点，新增节点设为黑色
		// insert_case2:新增节点的父节点是黑色，新增节点为红色没有破坏性质
		// insert_case3:父节点和叔父节点都为红色（此时一定有祖父节点，因为根节点不能红色）
		//				令父节点和叔父节点为黑色，祖父节点为红色（有可能破坏性质），继续从insert_case1的情形处理祖父节点
		// insert_case4:父节点为红色，叔父节点为黑色或缺少，新节点与父节点在异侧（比如新节点是父节点的左子树，父节点是祖父节点的右子树）
		//              父节点为祖父节点的左（右）子节点时，左（右）旋当前节点（旋转后当前节点成为父节点），然后以insert_case5处理旋转前的父节点
		// insert_case5:父节点为红色（祖父节点一定为黑色），叔父节点为黑色或缺少,新节点与父节点在同侧，
		//              同在左（右）侧时，右（左）旋父节点（父节点成为祖父节点），交换旋转前祖父节点（黑）和父节点（红）的颜色
		static void insert_case(base_ptr ptr, base_ptr& root)
		{
			ptr->color = Red;
			if (ptr == root) { //insert_case1
				ptr->color = Black;
				return;
			}
			if (ptr->parent->color == Red) {
				base_ptr uncle = ptr->uncle();
				if (uncle != nullptr && uncle->color == Red) { //insert_case3
					ptr->parent->color = uncle->color = Black;
					ptr->grandparent()->color = Red;
					insert_case(ptr->grandparent(),root);
				}
				else {
					if (ptr->parent->right == ptr && ptr->grandparent()->left == ptr->parent) {
						//insert_case4(子右，父左)，左旋ptr
						rotate_left(ptr, root);
						//直接执行insert_case5（同在左侧）
						ptr->color = Black;
						ptr->parent->color = Red;
						rotate_right(ptr, root);
					}
					else if (ptr->parent->left == ptr && ptr->grandparent()->right == ptr->parent) {
						//insert_case4(子左，父右)，右旋ptr
						rotate_right(ptr, root);
						//直接执行insert_case5（同在右侧）
						ptr->color = Black;
						ptr->parent->color = Red;
						rotate_left(ptr, root);
					}
					else if (ptr->parent->left == ptr && ptr->grandparent()->left == ptr->parent) {
						//insert_case5（同在左侧）
						ptr->parent->color = Black;
						ptr->grandparent()->color = Red;
						rotate_right(ptr->parent, root);
					}
					else if (ptr->parent->right == ptr && ptr->grandparent()->right == ptr->parent) {
						//insert_case5（同在右侧）
						ptr->parent->color = Black;
						ptr->grandparent()->color = Red;
						rotate_left(ptr->parent, root);
					}
				}
			}
		}

		//处理待删除节点为黑色节点，且没有子节点的情况
		//删除此节点后，此节点的一边黑色节点就少了一个，破坏了性质5
		//此时一定有兄弟节点，因为如果没有兄弟节点就破坏了性质5
		static void delete_case(base_ptr ptr, base_ptr& root) {
			base_ptr bro = ptr->brother();
			base_ptr father = ptr->parent;

			//兄弟节点为黑色
			if (bro->color == Black) {
				//如果兄弟节点如果有子节点，一定为红色（不然就破坏了性质5）
				if (bro->left != nullptr || bro->right != nullptr) {
					//bro为右子节点
					if (bro == ptr->parent->right) {
						//有同方向的红色节点
						if (bro->right != nullptr) {
							//bro成为新的父节点，并继承原父节点的颜色（原父节点的颜色无所谓）
							//原父节点和同向红色子节点设为黑色
							rotate_left(bro, root);
							bro->right->color = Black;
							bro->color = father->color;
							father->color = Black;
						}
						//没有同方向的红色节点
						else {
							//旋转到同方向
							bro->left->color = Black;
							bro->color = Red;
							rotate_right(bro->left, root);
							delete_case(ptr, root); //重新调用
						}
					}
					//bro为左子节点
					else {
						//有同方向的红色节点
						if (bro->left != nullptr) {
							//bro成为新的父节点，并继承原父节点的颜色（原父节点的颜色无所谓）
							//原父节点和同向红色子节点设为黑色
							rotate_right(bro, root);
							bro->color = father->color;
							bro->left->color = Black;
							father->color = Black;
						}
						//没有同方向的红色节点
						else {
							//旋转到同方向
							bro->right->color = Black;
							bro->color = Red;
							rotate_right(bro->right, root);
							delete_case(ptr, root); //重新调用
						}
					}
				}
				//兄弟节点没有子节点
				else {
					//父节点为红色，互换father与bro的颜色
					if (father->color == Red) {
						father->color = Black;
						bro->color = Red;
					}
					//父节点为黑色，这种情况下，bro一边黑色节点会多出一个
					//将bro设为红色，此时原来通过bro的叶子路径少了一个黑色节点，
					//因此将father当做待删除节点继续处理（就好像father这一边少了一个黑色节点，因为father的左右黑色节点要么被删除，要么变为红色）
					else {
						bro->color = Red;
						delete_case(father,root);
					}
				}
			}
			//兄弟节点为红色，一定有两个黑色子节点（性质5），且父节点一定为黑色（否则破坏性质4）
			else {
				//旋转使bro变为父节点
				if (bro == father->right) {
					rotate_left(bro, root);
				}
				else {
					rotate_right(bro, root);
				}

				bro->color = Black;
				father->color = Red;
				//此时ptr的兄弟节点一定为bro的其中一个黑色子节点
				//继续处理
				delete_case(ptr,root);
			}

		}//end of delete_case

	};//end of rb_tree_node_base

	


	template<typename T>
	struct rb_tree_node :public rb_tree_node_base {
		using node_ptr = rb_tree_node<T>*;
		T value;

		rb_tree_node(const T& val)
			:value(val)
		{
		}

		rb_tree_node(T&& val)
			:value(std::move(val))
		{
		}
	};

	template<typename T>
	struct rb_tree_const_iteartor : public mystl::iterator<mystl::bidirectional_iterator_tag, T> {
		using node_ptr = typename rb_tree_node<T>::node_ptr;
		using base_ptr = rb_tree_node_base*;
		using self = rb_tree_const_iteartor<T>;

		using value_type = T;
		using pointer = const T*;
		using reference = const T&;

		base_ptr base_p;

		rb_tree_const_iteartor() :base_p(nullptr) {}
		rb_tree_const_iteartor(base_ptr p) :base_p(p) {}


		reference operator*() const noexcept {
			return static_cast<node_ptr>(base_p)->value;
		}

		pointer operator->() const noexcept {
			return &(operator*());
		}

		self& operator++()
		{
			base_p = base_p->successor();
			return *this;
		}
		self operator++(int)
		{
			self tmp(*this);
			base_p = base_p->successor();
			return tmp;
		}
		self& operator--()
		{
			base_p = base_p->precursor();
			return *this;
		}
		self operator--(int)
		{
			self tmp(*this);
			base_p = base_p->precursor();
			return tmp;
		}

		bool operator==(const self& rhs) const { return base_p == rhs.base_p; }
		bool operator!=(const self& rhs) const { return base_p != rhs.base_p; }
	};

	template<typename T>
	struct rb_tree_iterator : public rb_tree_const_iteartor<T> {
		using node_ptr = typename rb_tree_node<T>::node_ptr;
		using base_ptr = rb_tree_node_base*;
		using self = rb_tree_iterator<T>;

		using value_type = T;
		using pointer = T*;
		using reference = T&;

		using rb_tree_const_iteartor<T>::base_p; //使用基类的成员

		rb_tree_iterator() = default;

		rb_tree_iterator(base_ptr p):rb_tree_const_iteartor<T>(p) {}

		reference operator*() const noexcept {
			return static_cast<node_ptr>(base_p)->value;
		}

		pointer operator->() const noexcept {
			return &(operator*());
		}

		self& operator++()
		{
			base_p = base_p->successor();
			return *this;
		}
		self operator++(int)
		{
			self tmp(*this);
			base_p = base_p->successor();
			return tmp;
		}
		self& operator--()
		{
			base_p = base_p->precursor();
			return *this;
		}
		self operator--(int)
		{
			self tmp(*this);
			base_p = base_p->precursor();
			return tmp;
		}
	};


	/*叶子的定义图示:
	           3黑
	         /    \
	        1黑    5黑
	       / \     / \
	    叶子 叶子 4红 叶子
	             / \
	          叶子 叶子

	1.节点是红色或黑色。
	2.根是黑色。
	3.所有叶子都是黑色。
	4.每个红色节点必须有两个黑色的子节点。（从每个叶子到根的所有路径上不能有两个连续的红色节点。）
	5.从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。

	在此红黑树的实现中，叶子并没有用实际节点表示*/


	template <typename T, typename Compare, typename Alloc = mystl::allocator<T>>
	class rb_tree {
	public:
		//T不为pair时，key_type,mapped_type,value_type相同
		//T为pair时，key_type为pair的第一个元素,mapped_type为第二个元素,value_type为pair
		using value_traits = rb_tree_value_traits<T>;

		using key_type = typename value_traits::key_type;
		using mapped_type = typename value_traits::mapped_type;
		using value_type = typename value_traits::value_type;
		using key_compare = Compare;

		using allocator_type = Alloc;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;

		using iterator = rb_tree_iterator<T>;
		using const_iterator = rb_tree_const_iteartor<T>;
		using reverse_iterator = mystl::reverse_iterator<iterator>;
		using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

		using base_type = rb_tree_node_base;
		using base_ptr = base_type*;
		using node_type = rb_tree_node<T>;
		using node_ptr = node_type*;

		using base_allocator = mystl::allocator<base_type>;
		using node_allocator = mystl::allocator<node_type>;
		using data_allocator = Alloc;

		allocator_type get_allocator() const { return data_allocator(); }
		key_compare key_comp() const { return key_comp_; }

	private:
		//end()为header_,--end()为max_node(),begin()为min_node()
		base_ptr    header_;       //与根节点互为对方的父节点
		size_type   node_count_;   //节点数
		key_compare key_comp_;    //key_type的比较规则
	private:
		base_ptr& root()      const { return header_->parent; } //根节点
		base_ptr& min_node()  const { return header_->left; } //最小节点
		base_ptr& max_node() const { return header_->right; }//最大节点

		const key_type& node_key(base_ptr ptr) const { return value_traits::get_key(static_cast<node_ptr>(ptr)->value); }//获取节点的key
		const key_type& node_key(node_ptr ptr) const { return value_traits::get_key(ptr->value); }
		const value_type& node_value(base_ptr ptr)const { return value_traits::get_value(static_cast<node_ptr>(ptr)->value); }//获取节点的value
	private:
		template<typename ...Args>
		node_ptr create_node(Args... args);
		node_ptr clone_node(node_ptr ptr);
		void desotry_node(node_ptr ptr);
		void init_header();

		base_ptr copy_tree(base_ptr other);//递归拷贝整颗树
		void delete_tree(node_ptr ptr);//递归删除整颗树

		//返回插入点的父节点以及父节点的左(true)/右(false)
		std::pair<base_ptr, bool> find_insert_pos(const key_type& key);
		//在find_insert_pos的基础上检查是否存在key,存在则第一个bool为false
		std::pair<bool,std::pair<base_ptr, bool>> find_insert_unique_pos(const key_type& key);

		//插入节点到树中
		//pos_parent:插入点的父节点
		//parent_left: true:在父节点的左子节点插入 false:在右子节点插入
		//返回插入点的迭代器
		iterator __insert_node(base_ptr pos_parent, bool parent_left, node_ptr new_node);

		//删除节点
		void __erase(base_ptr ptr);
		
	public:
		rb_tree(const Compare& comp = Compare()) 
			:node_count_(0),key_comp_(comp) 
		{ init_header(); }
		rb_tree(const rb_tree& other);
		rb_tree(rb_tree&& other) noexcept;

		rb_tree& operator=(const rb_tree& other);
		rb_tree& operator=(rb_tree&& other)noexcept;

		~rb_tree() 
		{ 
			clear(); 
			if (header_ != nullptr) {
				base_allocator::deallocate(header_, 1);
			}
		}
	public:
		//迭代器
		iterator begin()noexcept { return min_node(); }
		const_iterator begin()const noexcept { return min_node(); }
		const_iterator cbegin()const noexcept { return begin(); }
		iterator end()noexcept { return header_; };
		const_iterator end()const noexcept { return header_; }
		const_iterator cend()const noexcept { return end(); }

		reverse_iterator rbegin()noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin()const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin()const noexcept { return rbegin(); };
		reverse_iterator rend()noexcept { return reverse_iterator(begin()); };
		const_reverse_iterator rend()const noexcept { return const_reverse_iterator(begin()); };
		const_reverse_iterator crend()const noexcept { return rend(); };

		//容量
		bool      empty()    const noexcept { return node_count_ == 0; }
		size_type size()     const noexcept { return node_count_; }

	public:
		//修改器
		void clear();

		//insert
		std::pair<iterator, bool> insert_unique(const value_type& value);
		std::pair<iterator, bool> insert_unique(value_type&& value);
		//插入 value 到尽可能接近，hint 的位置。返回指向被插入元素，或阻止插入的元素的迭代器。
		iterator insert_unique(const_iterator hint, const value_type& value);
		iterator insert_unique(const_iterator hint, value_type&& value);
		template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int> = 0>
		void insert_unique(InputIt first, InputIt last);
		

		iterator insert_multi(const value_type& value);
		iterator insert_multi(value_type&& value);
		//插入 value 到尽可能接近，hint 的位置。返回指向新插入元素的迭代器。
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
		iterator emplace_multi_hint(const_iterator hint,Args&& ...args);

		//erase
		iterator erase(const_iterator pos);

		size_type erase_multi(const key_type& key);
		size_type erase_unique(const key_type& key);

		iterator erase(const_iterator first, const_iterator last);

	public:
		//操作
		iterator find(const key_type& key);
		const_iterator find(const key_type& key) const;

		size_type count(const key_type& key) const;

		iterator lower_bound(const key_type& key);
		const_iterator lower_bound(const key_type& key) const;

		iterator upper_bound(const key_type& key);
		const_iterator upper_bound(const key_type& key) const;

		std::pair<iterator, iterator> equal_range(const key_type& key);
		std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

		void swap(rb_tree& other) noexcept;
	};

	template<typename T, typename Compare, typename Alloc>
	template<typename ...Args>
	inline typename rb_tree<T, Compare, Alloc>::node_ptr
		rb_tree<T, Compare, Alloc>::create_node(Args ...args)
	{
		node_ptr new_node = node_allocator::allocate(1);
		mystl::construct(std::addressof(new_node->value), std::forward<Args>(args)...);
		new_node->parent = nullptr;
		new_node->left = nullptr;
		new_node->right = nullptr;
		return new_node;
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::node_ptr
		rb_tree<T, Compare, Alloc>::clone_node(node_ptr ptr)
	{
		//复制value和color
		node_ptr node_p = create_node(ptr->value);
		node_p->color = ptr->color;
		return node_p;
	}

	template<typename T, typename Compare, typename Alloc>
	inline void rb_tree<T, Compare, Alloc>::desotry_node(node_ptr ptr)
	{
		mystl::destroy_at(std::addressof(ptr->value));
		node_allocator::deallocate(ptr, 1);
	}

	template<typename T, typename Compare, typename Alloc>
	inline void rb_tree<T, Compare, Alloc>::init_header()
	{
		header_ = base_allocator::allocate(1);
		header_->color = Red; //根节点一定是黑色，header_设置为红色用于区分（用于precursor中）
		root() = nullptr;
		min_node() = header_;
		max_node() = header_;
	}

	template<typename T, typename Compare, typename Alloc>
	typename rb_tree<T, Compare, Alloc>::base_ptr
		rb_tree<T, Compare, Alloc>::copy_tree(base_ptr other)
	{
		if (other == nullptr)
			return nullptr;

		node_ptr ptr_copy = clone_node(static_cast<node_ptr>(other));
		ptr_copy->left = copy_tree(other->left);
		ptr_copy->right = copy_tree(other->right);

		if (ptr_copy->left != nullptr)
			ptr_copy->left->parent = ptr_copy;
		if (ptr_copy->right != nullptr)
			ptr_copy->right->parent = ptr_copy;

		return ptr_copy;
	}

	//后序遍历删除节点
	template<typename T, typename Compare, typename Alloc>
	void rb_tree<T, Compare, Alloc>::delete_tree(node_ptr ptr)
	{
		if (ptr == nullptr)
			return;
		delete_tree(static_cast<node_ptr>(ptr->left));
		delete_tree(static_cast<node_ptr>(ptr->right));
		desotry_node(ptr);
	}

	template<typename T, typename Compare, typename Alloc>
	inline std::pair<typename rb_tree<T, Compare, Alloc>::base_ptr, bool> 
		rb_tree<T, Compare, Alloc>::find_insert_pos(const key_type& key)
	{
		base_ptr pos_parent = header_; //插入点的父节点
		base_ptr pos = root(); //插入点

		//先用二叉搜索树的方式找到插入点
		bool parent_left = true; //插入点是否是父节点的左子节点
		while (pos != nullptr) {
			pos_parent = pos;
			parent_left = key_comp_(key, node_key(pos));
			pos = parent_left ? pos->left : pos->right;  //小于往左移，大于等于往右移
		}
		return std::make_pair(pos_parent, parent_left);
	}

	template<typename T, typename Compare, typename Alloc>
	inline std::pair<bool, std::pair<typename rb_tree<T, Compare, Alloc>::base_ptr, bool>> 
		rb_tree<T, Compare, Alloc>::find_insert_unique_pos(const key_type& key)
	{
		auto p = find_insert_pos(key);
		base_ptr pos_parent = p.first;
		bool parent_left = p.second;
		base_ptr tmp = pos_parent;

		if (parent_left) {
			//在最小值左边插入，必定没有相同值
			if (pos_parent == min_node())
				return std::make_pair(true,p);
			else {
				tmp = tmp->precursor(); //如果之前存在相同的key，则现在父节点的前驱节点一定是相同的key
			}
		}

		if (key_comp_(node_key(tmp), key)) {
			//父节点的key小于要插入节点的key，则没有相同值，可以插入
			return std::make_pair(true,p);
		}
		else {
			return std::make_pair(false,std::make_pair(tmp,pos_parent));//有相同值，无法插入，返回相同值的节点
		}
	}

	template<typename T, typename Compare, typename Alloc>
	typename rb_tree<T, Compare, Alloc>::iterator
	rb_tree<T, Compare, Alloc>::__insert_node(base_ptr pos_parent, bool parent_left, node_ptr new_node)
	{
		new_node->parent = pos_parent;
		if (pos_parent == header_) { //树为空，插入的第一个节点
			root() = new_node;
			min_node() = new_node;
			max_node() = new_node;
		}
		else if (parent_left) {
			pos_parent->left = new_node;
			if (pos_parent == min_node()) {
				min_node() = new_node;
			}
		}
		else {//右边插入
			pos_parent->right = new_node;
			if (pos_parent == max_node()) {
				max_node() = new_node;
			}
		}
		//红黑树自平衡
		base_type::insert_case(new_node,root());
		++node_count_;
		return iterator(new_node);
	}

	//删除一个节点，进入此方法的节点最多只有一边有子节点
	template<typename T, typename Compare, typename Alloc>
	void rb_tree<T, Compare, Alloc>::__erase(base_ptr ptr)
	{
		//选择子节点
		base_ptr child = ptr->left == nullptr ? ptr->right : ptr->left;

		//树只有一个节点时
		if (ptr == root() && child == nullptr) {
			desotry_node(static_cast<node_ptr>(ptr));
			--node_count_;
			root() = nullptr;
			min_node() = header_;
			max_node() = header_;
			return;
		}

		//待删除节点没有子节点
		if (child == nullptr) {
			//待删除节点为红色，直接删除，不会破坏性质
			if (ptr->color == Red) {
			}
			else { 
				//待删除节点为黑色，比较复杂
				base_type::delete_case(ptr,root()); 
			}
			if (min_node() == ptr) {
				min_node() = ptr->parent;
			}
			else if (max_node() == ptr) {
				max_node() = ptr->parent;
			}
		}
		else {
		//如果待删除节点有子节点,则待删除节点一定为黑色，子节点一定为红色
		//因为其它组合都会破坏红黑树的性质。用p指待删除节点，s指子节点
		//1: p为红，s为红  破坏性质4
		//2: p为红，s为黑  破坏性质5
		//3：p为黑，s为黑  破坏性质5
		// 
		//因此只需要将子节点设为黑色，然后顶替上来就行了

			child->color = Black;
			if (root() == ptr) {//删除根节点
				root() = child;
			}
			if (min_node() == ptr) {
				min_node() = child;
			}
			else if (max_node() == ptr) {
				max_node() = child;
			}
		}
		//子节点顶替待删除节点
		if (ptr->parent->left == ptr) {
			ptr->parent->left = child;
		}
		else{
			ptr->parent->right = child;
		}
		if (child != nullptr) {
			child->parent = ptr->parent;
		}
		desotry_node(static_cast<node_ptr>(ptr));
		--node_count_;
	}

	template<typename T, typename Compare, typename Alloc>
	inline rb_tree<T, Compare, Alloc>::rb_tree(const rb_tree& other)
	{
		init_header();
		if (other.node_count_ != 0)
		{
			root() = copy_tree(other.root());
			root()->parent = header_;
			min_node() = base_type::min_node(root());
			max_node() = base_type::max_node(root());
		}
		node_count_ = other.node_count_;
		key_comp_ = other.key_comp_;
	}


	template<typename T, typename Compare, typename Alloc>
	inline rb_tree<T, Compare, Alloc>::rb_tree(rb_tree&& other) noexcept
		:header_(other.header_),
		node_count_(other.node_count_),
		key_comp_(other.key_comp_)
	{
		other.header_ = nullptr;
		other.node_count_ = 0;
	}

	template<typename T, typename Compare, typename Alloc>
	inline rb_tree<T, Compare, Alloc>&
		rb_tree<T, Compare, Alloc>::operator=(const rb_tree& other)
	{
		if (this != &other) {
			clear();
			if (other.node_count_ != 0) {
				root() = copy_tree(other.root());
				root()->parent = header_;
				min_node() = base_type::min_node(root());
				max_node() = base_type::max_node(root());
			}
			node_count_ = other.node_count_;
			key_comp_ = other.key_comp_;
		}
		return *this;
	}

	template<typename T, typename Compare, typename Alloc>
	inline rb_tree<T, Compare, Alloc>&
		rb_tree<T, Compare, Alloc>::operator=(rb_tree&& other)noexcept
	{
		if (this != &other) {
			clear();
			if (header_ != nullptr) {
				base_allocator::deallocate(header_, 1);
			}
			header_ = other.header_;
			node_count_ = other.node_count_;
			key_comp_ = other.key_comp_;
			other.header_ = nullptr;
			other.node_count_ = 0;
		}
		return *this;
	}



	template<typename T, typename Compare, typename Alloc>
	inline void rb_tree<T, Compare, Alloc>::clear()
	{
		if (node_count_ != 0) {
			delete_tree(static_cast<node_ptr>(root()));
			node_count_ = 0;
			root() = nullptr;
			min_node() = header_;
			max_node() = header_;
		}
	}

	//insert
	template<typename T, typename Compare, typename Alloc>
	std::pair<typename rb_tree<T, Compare, Alloc>::iterator, bool> 
		rb_tree<T, Compare, Alloc>::insert_unique(const value_type& value)
	{
		
		const key_type& key = value_traits::get_key(value);

		std::pair<bool,std::pair<base_ptr,bool>> p = find_insert_unique_pos(key);
		if (p.first == false) {
			return std::make_pair(iterator(p.second.first), false);//有相同值，无法插入，返回相同值的迭代器位置
		}
		else {
			return std::make_pair(__insert_node(p.second.first, p.second.second, create_node(value)), true);
		}	
	}

	template<typename T, typename Compare, typename Alloc>
	inline std::pair<typename rb_tree<T, Compare, Alloc>::iterator, bool> 
		rb_tree<T, Compare, Alloc>::insert_unique(value_type&& value)
	{
		const key_type& key = value_traits::get_key(value);

		std::pair<bool, std::pair<base_ptr, bool>> p = find_insert_unique_pos(key);
		if (p.first == false) {
			return std::make_pair(iterator(p.second.first), false);//有相同值，无法插入，返回相同值的迭代器位置
		}
		else {
			return std::make_pair(__insert_node(p.second.first, p.second.second, create_node(std::move(value))), true);
		}
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::insert_unique(const_iterator hint, const value_type& value)
	{
		return emplace_unique_hint(hint, value); //如果节点存在，会浪费一次创建和删除节点的时间
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::insert_unique(const_iterator hint, value_type&& value)
	{
		return emplace_unique_hint(hint, std::move(value)); //如果节点存在，会浪费一次创建和删除节点的时间
	}

	template<typename T, typename Compare, typename Alloc>
	template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void rb_tree<T, Compare, Alloc>::insert_unique(InputIt first, InputIt last)
	{
		while (first != last) {
			insert_unique(*first);
			++first;
		}
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator
		rb_tree<T, Compare, Alloc>::insert_multi(const value_type& value)
	{
		const key_type& key = value_traits::get_key(value);

		std::pair<base_ptr,bool> p = find_insert_pos(key);
		return __insert_node(p.first, p.second, create_node(value));
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::insert_multi(value_type&& value)
	{
		return emplace_multi(std::move(value));
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::insert_multi(const_iterator hint, const value_type& value)
	{
		return emplace_multi_hint(hint, value);
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::insert_multi(const_iterator hint, value_type&& value)
	{
		return emplace_multi_hint(hint,std::move(value));
	}

	template<typename T, typename Compare, typename Alloc>
	template<typename InputIt, std::enable_if_t<mystl::is_input_iter_v<InputIt>, int>>
	inline void rb_tree<T, Compare, Alloc>::insert_multi(InputIt first, InputIt last)
	{
		while (first != last) {
			insert_multi(*first);
			++first;
		}
	}

	//emplace
	template<typename T, typename Compare, typename Alloc>
	template<typename ...Args>
	inline std::pair<typename rb_tree<T, Compare, Alloc>::iterator, bool>
		rb_tree<T, Compare, Alloc>::emplace_unique(Args && ...args)
	{
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);

		std::pair<bool, std::pair<base_ptr, bool>> p = find_insert_unique_pos(key);
		if (p.first == false) {
			desotry_node(new_node);  //销毁创建的节点
			return std::make_pair(iterator(p.second.first), false);//有相同值，无法插入，返回相同值的迭代器位置
		}
		else {
			return std::make_pair(__insert_node(p.second.first, p.second.second, new_node), true);
		}	
	}

	template<typename T, typename Compare, typename Alloc>
	template<typename ...Args>
	typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::emplace_unique_hint(const_iterator hint, Args && ...args)
	{
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);
		base_ptr hint_node = hint.base_p;

		if (node_count_ == 0) {
			return __insert_node(header_, true, new_node); //第一个节点在header左边插入
		}
		if (hint == begin()) {
			//权值比最小的节点小，在最小节点左边插入
			if (key_comp_(key, node_key(hint_node))) {
				return __insert_node(hint_node, true, new_node);
			}
			else {
				auto p = find_insert_unique_pos(key);
				if (p.first == false) { //有相同值
					desotry_node(new_node);  //销毁创建的节点
					return iterator(p.second.first); //有相同值，无法插入，返回相同值的迭代器位置
				}
				else {
					return __insert_node(p.second.first, p.second.second, new_node);
				}
			}
		}
		else if (hint == end()) {
			//权值比最大的节点大，在最大节点右边插入
			if (key_comp_(node_key(max_node()), key)) {
				return __insert_node(max_node(), false, new_node);
			}
			else {
				auto p = find_insert_unique_pos(key);
				if (p.first == false) { //有相同值
					desotry_node(new_node);  //销毁创建的节点
					return iterator(p.second.first); //有相同值，无法插入，返回相同值的迭代器位置
				}
				else {
					return __insert_node(p.second.first, p.second.second, new_node);
				}
			}
		}
		else {//hint != begin() && hint != end()
			base_ptr hint_pre_node = hint_node->precursor(); //hint的前驱节点
			if (key_comp_(node_key(hint_pre_node), key) && key_comp_(key, node_key(hint_node))) {
				//key的权值处于hint的前驱和hint之前
				//可以在前驱的右节点插入，或hint的左节点插入
				//如果前驱节点正好是hint的父节点就只能在hint的左节点插入了
				if (hint_pre_node->right == nullptr) {
					return __insert_node(hint_pre_node, false, new_node);
				}
				else {
					return __insert_node(hint_node, true, new_node);
				}
			}
			else {
				auto p = find_insert_unique_pos(key);
				if (p.first == false) { //有相同值
					desotry_node(new_node);  //销毁创建的节点
					return iterator(p.second.first); //返回相同值的迭代器位置
				}
				else {
					return __insert_node(p.second.first, p.second.second, new_node);
				}
			}
		}

		
	}

	template<typename T, typename Compare, typename Alloc>
	template<class ...Args>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::emplace_multi(Args && ...args)
	{
		
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);

		std::pair<base_ptr, bool> p = find_insert_pos(key);
		return __insert_node(p.first, p.second, new_node);
	}

	template<typename T, typename Compare, typename Alloc>
	template<typename ...Args>
	typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::emplace_multi_hint(const_iterator hint, Args && ...args)
	{
		node_ptr new_node = create_node(std::forward<Args>(args)...);
		const key_type& key = node_key(new_node);
		base_ptr hint_node = hint.base_p;

		if (node_count_ == 0) {
			return __insert_node(header_, true, new_node); //第一个节点在header左边插入
		}
		if (hint == begin()) {
			//权值比最小的节点小，在最小节点左边插入
			if (key_comp_(key, node_key(hint_node))) {
				return __insert_node(hint_node, true, new_node);
			}
			else {
				auto p = find_insert_pos(key);
				return __insert_node(p.first, p.second, new_node);
			}
		}
		else if (hint == end()) {
			//权值比最大的节点大，在最大节点右边插入
			if (key_comp_(node_key(max_node()), key)) {
				return __insert_node(max_node(), false, new_node);
			}
			else {
				auto p = find_insert_pos(key);
				return __insert_node(p.first, p.second, new_node);
			}
		}
		else {//hint != begin() && hint != end()
			base_ptr hint_pre_node = hint_node->precursor(); //hint的前驱节点
			if (key_comp_(node_key(hint_pre_node), key) && key_comp_(key, node_key(hint_node))) {
				//key的权值处于hint的前驱和hint之前
				//可以在前驱的右节点插入，或hint的左节点插入
				//如果前驱节点正好是hint的父节点就只能在hint的左节点插入了
				if (hint_pre_node->right == nullptr) {
					return __insert_node(hint_pre_node, false, new_node);
				}
				else {
					return __insert_node(hint_node, true, new_node);
				}
			}
			else {
				auto p = find_insert_pos(key);
				return __insert_node(p.first, p.second, new_node);
			}
		}
	}

	//erase
	//如果节点存在返回值为其后继节点
	//不存在返回end()
	template<typename T, typename Compare, typename Alloc>
	typename rb_tree<T, Compare, Alloc>::iterator
		rb_tree<T, Compare, Alloc>::erase(const_iterator pos)
	{
		base_ptr ptr = pos.base_p;

		const key_type& key = node_key(ptr);
		//先用二插搜索树的方式选择要删除的节点
		base_ptr curr = root();
		while (curr != nullptr) {
			const key_type& curr_key = node_key(curr);
			if (key_comp_(key, curr_key)) { //目标key权值更小，往左搜索
				curr = curr->left;
			}
			else if (key_comp_(curr_key, key)) {//目标key权值更大，往右搜索
				curr = curr->right;
			}
			else {//目标key等于当前key
				if (curr->right == nullptr) {
					//没有右子树
					iterator ret(curr->successor());
					__erase(curr); //真正删除节点
					return ret;
				}
				else {
					//有右子树
					//找后继节点,并交换后继节点和当前节点的值
					base_ptr succ= curr->successor(); //后继节点

					mystl::swap(const_cast<value_type&>(node_value(curr)), const_cast<value_type&>(node_value(succ)));

					__erase(succ);//真正删除节点
					//curr存放的就是后继节点的value
					return iterator(curr);
				}
			}
		}
		return end(); //不存在对应节点
	}

	//删除所有等于key的节点
	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::size_type
		rb_tree<T, Compare, Alloc>::erase_multi(const key_type& key)
	{
		auto p = equal_range(key); 
		size_type n = mystl::distance(p.first, p.second);
		erase(p.first, p.second);
		return n;
	}

	//删除一个等于key的节点
	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::size_type
		rb_tree<T, Compare, Alloc>::erase_unique(const key_type& key)
	{
		iterator it = find(key);
		if (it != end())
		{
			erase(it);
			return 1;
		}
		return 0;
	}

	//删除[first,last)区间内的节点
	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::erase(const_iterator first, const_iterator last)
	{
		if (first == begin() && last == end()){
			clear();
		}
		else{
			size_type n = mystl::distance(first, last);
			while (n) {
				first = erase(first); //删除节点可能会使后继节点失效，erase返回有效的后继节点
				--n;
			}
		}
		return iterator(first.base_p);
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::find(const key_type& key)
	{
		//直接调用const版本的find
		const_iterator it = static_cast<const rb_tree<T, Compare, Alloc>&>(*this).find(key);
		return iterator(it.base_p);
	}

	template<typename T, typename Compare, typename Alloc>
	typename rb_tree<T, Compare, Alloc>::const_iterator 
		rb_tree<T, Compare, Alloc>::find(const key_type& key) const
	{
		base_ptr curr = root();
		while (curr != nullptr) {
			const key_type& curr_key = node_key(curr);
			if (key_comp_(key, curr_key)) {//目标key权值更小
				curr = curr->left;
			}
			else if (key_comp_(curr_key, key)) {//目标key权值更大
				curr = curr->right;
			}
			else {
				return const_iterator(curr);
			}
		}
		return end();
	}

	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::size_type 
		rb_tree<T, Compare, Alloc>::count(const key_type& key) const
	{
		auto p = equal_range(key);
		return static_cast<size_type>(mystl::distance(p.first, p.second));
	}

	//返回第一个不小于key的迭代器
	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator 
		rb_tree<T, Compare, Alloc>::lower_bound(const key_type& key)
	{
		//直接调用const版本的lower_bound
		const_iterator it = static_cast<const rb_tree<T, Compare, Alloc>&>(*this).lower_bound(key);
		return iterator(it.base_p);
	}

	template<typename T, typename Compare, typename Alloc>
	typename rb_tree<T, Compare, Alloc>::const_iterator 
		rb_tree<T, Compare, Alloc>::lower_bound(const key_type& key) const
	{
		base_ptr ret = header_;
		base_ptr curr = root();
		while (curr != nullptr) {
			const key_type& curr_key = node_key(curr);
			if (!key_comp_(curr_key,key)) {
				//curr_key的权值不小于key
				ret = curr;       //记录当前节点
				curr = curr->left;
			}
			else {
				//curr_key的权值小于key，继续向右找第一个不小于key的节点
				curr = curr->right;
			}
		}
		return const_iterator(ret);
	}

	//返回第一个大于key的迭代器
	template<typename T, typename Compare, typename Alloc>
	inline typename rb_tree<T, Compare, Alloc>::iterator
		rb_tree<T, Compare, Alloc>::upper_bound(const key_type& key)
	{
		//直接调用const版本的upper_bound
		const_iterator it = static_cast<const rb_tree<T, Compare, Alloc>&>(*this).upper_bound(key);
		return iterator(it.base_p);
	}

	template<typename T, typename Compare, typename Alloc>
	typename rb_tree<T, Compare, Alloc>::const_iterator 
		rb_tree<T, Compare, Alloc>::upper_bound(const key_type& key) const
	{
		base_ptr ret = header_;
		base_ptr curr = root();
		while (curr != nullptr) {
			const key_type& curr_key = node_key(curr);
			if (key_comp_(key, curr_key)) {
				//当前key权值更小
				ret = curr;       //记录当前节点
				curr = curr->left;
			}
			else {
				//当前key >= curr_key,继续向右找第一个大于它的节点
				curr = curr->right;
			}
		}
		return const_iterator(ret);
	}

	//返回节点的key与目标key相等的节点范围
	template<typename T, typename Compare, typename Alloc>
	inline std::pair<typename rb_tree<T, Compare, Alloc>::iterator, typename rb_tree<T, Compare, Alloc>::iterator> 
		rb_tree<T, Compare, Alloc>::equal_range(const key_type& key)
	{
		//直接调用const版本的equal_range
		auto p = static_cast<const rb_tree<T, Compare, Alloc>&>(*this).equal_range(key);
		return std::make_pair(iterator(p.first.base_p), iterator(p.second.base_p));
	}

	template<typename T, typename Compare, typename Alloc>
	inline std::pair<typename rb_tree<T, Compare, Alloc>::const_iterator, typename rb_tree<T, Compare, Alloc>::const_iterator> 
		rb_tree<T, Compare, Alloc>::equal_range(const key_type& key) const
	{
		const_iterator low = lower_bound(key);
		if (low == end()) { //不存在对应key
			return std::make_pair(end(), end());
		}
		else {
			return std::make_pair(low, upper_bound(key));
		}
	}

	template<typename T, typename Compare, typename Alloc>
	inline void rb_tree<T, Compare, Alloc>::swap(rb_tree& other) noexcept
	{
		if (this != &other) {
			mystl::swap(header_, other.header_);
			mystl::swap(node_count_, other.node_count_);
		}
	}


	//非成员函数
	template <typename T, typename Compare, typename Alloc>
	bool operator==(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs)
	{
		return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <typename T, typename Compare, typename Alloc>
	bool operator<(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs)
	{
		return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <typename T, typename Compare, typename Alloc>
	bool operator!=(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T, typename Compare, typename Alloc>
	bool operator>(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template <typename T, typename Compare, typename Alloc>
	bool operator<=(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template <typename T, typename Compare, typename Alloc>
	bool operator>=(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <typename T, typename Compare, typename Alloc>
	void swap(const rb_tree<T, Compare, Alloc>& lhs, const rb_tree<T, Compare, Alloc>& rhs) noexcept {
		lhs.swap(rhs);
	}


}//end of namespace mystl
#endif // !_MY_RB_TREE_H_
