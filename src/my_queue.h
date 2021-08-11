#ifndef _MY_QUEUE_H_
#define _MY_QUEUE_H_

#include"my_deque.h" //for queue
#include"my_vector.h" //for priority_queue
#include"my_algorithm.h"

/*
此头文件包含先进先出队列queue
和优先队列priority_queue
*/
namespace mystl {
	template<typename T, typename Container = mystl::deque<T>>
	class queue {
	public:
		using container_type = Container;
		using value_type = typename Container::value_type;
		using size_type = typename Container::size_type;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;

	private:
		container_type c_;

	public:
		queue() : queue(Container()) { }

		explicit queue(const Container& cont)
			:c_(cont) { }

		explicit queue(Container&& cont)
			:c_(std::move(cont)) { }

		queue(const queue& other)
			:c_(other.c_) { }

		queue(queue&& other) noexcept
			:c_(std::move(other.c_)) { }

		~queue() = default;

		queue& operator=(const queue& other)
		{
			c_ = other.c_;
			return *this;
		}
		queue& operator=(queue&& other)
		{
			c_ = std::move(other.c_);
			return *this;
		}

	public:

		//元素访问
		reference front() { return c_.front(); }
		const_reference front() const { return c_.front(); }

		reference back() { return c_.back(); }
		const_reference back() const { return c_.back(); }


		//容量
		bool empty() const { return c_.empty(); }

		size_type size() const { return c_.size(); }

		//修改器

		void push(const value_type& value) { c_.push_back(value); }

		void push(value_type&& value) { c_.push_back(std::move(value)); }

		template< class... Args >
		void emplace(Args&&... args) { c_.emplace_back(std::forward<Args>(args)...); }

		void pop() { c_.pop_front(); }

		void swap(queue& other) noexcept(noexcept(mystl::swap(c_, other.c_)))
		{
			mystl::swap(c_, other.c_);
		}

		friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.c_ == rhs.c_; }
		friend bool operator< (const queue& lhs, const queue& rhs) { return lhs.c_ < rhs.c_; }
	};


	//非成员函数
	template< class T, class Container >
	bool operator==(const mystl::queue<T, Container>& lhs, const mystl::queue<T, Container>& rhs)
	{
		return lhs == rhs;
	}
	template< class T, class Container >
	bool operator!=(const mystl::queue<T, Container>& lhs, const mystl::queue<T, Container>& rhs)
	{
		return !(lhs == rhs);
	}
	template< class T, class Container >
	bool operator<(const mystl::queue<T, Container>& lhs, const mystl::queue<T, Container>& rhs)
	{
		return lhs < rhs;
	}

	template< class T, class Container >
	bool operator<=(const mystl::queue<T, Container>& lhs, const mystl::queue<T, Container>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class T, class Container >
	bool operator>(const mystl::queue<T, Container>& lhs, const mystl::queue<T, Container>& rhs)
	{
		return rhs < lhs;
	}

	template< class T, class Container >
	bool operator>=(const mystl::queue<T, Container>& lhs, const mystl::queue<T, Container>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class T, class Container >
	void swap(mystl::queue<T, Container>& lhs, mystl::queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}
	/***************************************************************************
	*				             priority_queue                                *
	***************************************************************************/

	template<typename T, typename Container = mystl::vector<T>,
	typename Compare = std::less<typename Container::value_type>>
	class priority_queue {
	public:
		using container_type = Container;
		using value_compare = Compare;
		using value_type = typename Container::value_type;
		using size_type = typename Container::size_type;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;

	private:
		container_type c_;
		value_compare comp_;
	public:
		priority_queue() = default;

		explicit priority_queue(const Compare& compare)
			: c_(), comp_(compare)
		{

		}

		priority_queue(const Compare& compare, const Container& cont)
			:c_(cont), comp_(compare)
		{
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		priority_queue(const Compare& compare, Container&& cont)
			:c_(std::move(cont)), comp_(compare)
		{
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		priority_queue(const priority_queue& other)
			:c_(other.c_), comp_(other.comp_)
		{
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		priority_queue(priority_queue&& other)noexcept
			:c_(std::move(other.c_)), comp_(other.comp_)
		{
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		priority_queue(std::initializer_list<T> ilist) 
			:c_(ilist),comp_() {
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		template<typename InputIt>
		priority_queue(InputIt first, InputIt last)
			: c_(first, last), comp_()
		{
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		template< class InputIt >
		priority_queue(InputIt first, InputIt last, const Compare& compare)
			: c_(first, last), comp_(compare)
		{
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		template< class InputIt >
		priority_queue(InputIt first, InputIt last, const Compare& compare, const Container& cont)
			: c_(cont), comp_(compare)
		{
			c_.insert(c_.end().first, last);
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		template< class InputIt >
		priority_queue(InputIt first, InputIt last, const Compare& compare, Container&& cont)
			:c_(std::move(cont)), comp_(compare)
		{
			c_.insert(c_.end(), first, last);
			mystl::make_heap(c_.begin(), c_.end(), comp_);
		}

		~priority_queue() = default;

		priority_queue& operator=(const priority_queue& other)
		{
			c_ = other.c_;
			comp_ = other.comp_;
			mystl::make_heap(c_.begin(), c_.end(), comp_);
			return *this;
		}

		priority_queue& operator=(priority_queue&& other)
		{
			c_ = std::move(other.c_);
			comp_ = other.comp_;
			mystl::make_heap(c_.begin(), c_.end(), comp_);
			return *this;
		}

	public:
		//元素访问
		const_reference top() const { return c_.front(); }

		//容量
		bool empty() const { return c_.empty(); }

		size_type size() const { return c_.size(); }

		//修改器
		void push(const value_type& value)
		{
			c_.push_back(value);
			mystl::push_heap(c_.begin(), c_.end(), comp_);
		}

		void push(value_type&& value)
		{
			c_.push_back(std::move(value));
			mystl::push_heap(c_.begin(), c_.end(), comp_);
		}

		template< class... Args >
		void emplace(Args&&... args)
		{
			c_.emplace_back(std::forward<Args>(args)...);
			mystl::push_heap(c_.begin(), c_.end(), comp_);
		}

		void pop()
		{
			mystl::pop_heap(c_.begin(), c_.end(), comp_);
			c_.pop_back();
		}

		void swap(priority_queue& other) noexcept(noexcept(mystl::swap(c_, other.c_)) && noexcept(mystl::swap(comp_, other.comp_)))
		{
			mystl::swap(c_, other.c_);
			mystl::swap(comp_, other.comp_);
		}

	};

	//非成员函数
	template< class T, class Container, class Compare >
	void swap(mystl::priority_queue<T, Container, Compare>& lhs,
		mystl::priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}


}//end of namespace mystl

#endif // !_MY_QUEUE_H_