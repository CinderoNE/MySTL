#ifndef _MY_STACK_H_
#define _MY_STACK_H_

#include"my_deque.h"

//容器适配器stack
namespace mystl {
	template<typename T,typename Container = mystl::deque<T>>
	class stack {
	public:
		using container_type = Container;
		using value_type = typename Container::value_type;
		using size_type = typename Container::size_type;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;

	private:
		container_type c_;

	public:
		stack() : stack(Container()) { }

		explicit stack(const Container& cont)
			:c_(cont){ }

		explicit stack(Container&& cont)
			:c_(std::move(cont)){ }

		stack(const stack& other)
			:c_(other.c_){ }

		stack(stack&& other)noexcept
			:c_(std::move(other.c_)){ }

		~stack() = default;

		stack& operator=(const stack& other)
		{
			c_ = other.c_;
			return *this;
		}
		stack& operator=(stack&& other)
		{
			c_ = std::move(other.c_);
			return *this;
		}

	public:

		//元素访问
		reference top() { return c_.back(); }

		const_reference top() const { return c_.back(); }

		//容量
		bool empty() const { return c_.empty(); }

		size_type size() const { return c_.size(); }

		//修改器

		void push(const value_type& value) { c_.push_back(value); }

		void push(value_type&& value) { c_.push_back(std::move(value)); }

		template< class... Args >
		void emplace(Args&&... args) { c_.emplace_back(std::forward<Args>(args)...); }

		void pop() { c_.pop_back(); }

		void swap(stack& other) noexcept(noexcept(mystl::swap(c_, other.c_)))
		{
			mystl::swap(c_, other.c_);
		}

		friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.c_ == rhs.c_; }
		friend bool operator< (const stack& lhs, const stack& rhs) { return lhs.c_ < rhs.c_; }
	};


	//非成员函数
	template< class T, class Container >
	bool operator==(const mystl::stack<T, Container>& lhs, const mystl::stack<T, Container>& rhs)
	{
		return lhs == rhs;
	}
	template< class T, class Container >
	bool operator!=(const mystl::stack<T, Container>& lhs, const mystl::stack<T, Container>& rhs)
	{
		return !(lhs == rhs);
	}
	template< class T, class Container >
	bool operator<(const mystl::stack<T, Container>& lhs, const mystl::stack<T, Container>& rhs)
	{
		return lhs < rhs;
	}

	template< class T, class Container >
	bool operator<=(const mystl::stack<T, Container>& lhs, const mystl::stack<T, Container>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class T, class Container >
	bool operator>(const mystl::stack<T, Container>& lhs, const mystl::stack<T, Container>& rhs)
	{
		return rhs < lhs;
	}

	template< class T, class Container >
	bool operator>=(const mystl::stack<T, Container>& lhs, const mystl::stack<T, Container>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class T, class Container >
	void swap(mystl::stack<T, Container>& lhs, mystl::stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

} //end of namespace mystl


#endif // !_MY_STACK_H_
