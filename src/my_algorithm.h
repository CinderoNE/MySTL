#pragma once
#ifndef _MY_ALGORITHM_H_
#define _MY_ALGORITHM_H_

#include"my_iterator.h"

#include<cstring> //for memmove line:282
namespace mystl {

	template<typename T>
	inline const T& max(const T& a, const T& b) {
		return a < b ? b : a;
	}

	template<typename T>
	inline const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}

	//交换 a 与 b
	template<typename T>
	inline void swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}

	template <typename T>
	struct equal_to {
		bool operator()(const T& lhs, const T& rhs) const {
			return lhs == rhs;
		}
	};

	// 反转 [first, last) 范围中的元素顺序
	template<class BidirIt>
	void reverse(BidirIt first, BidirIt last) {
		while (first != last && first != --last) {
			mystl::swap(*first++, last);
		}
	}

	// 在范围 [first1, last1) 和始于 first2 的另一范围间交换元素。
	template< class ForwardIt1, class ForwardIt2 >
	inline ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1,
		ForwardIt2 first2) {
		while (first1 != last1) {
			mystl::swap(*first1, *first2);
		}
		return first2;
	}

	//如果范围 [first1, last1) 和范围 [first2, first2 + (last1 - first1)) 相等，返回 true ，否则返回 false
	template< class InputIt1, class InputIt2 >
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			if (*first1 != *first2) {
				return false;
			}
		}
		return true;
	}

	//复制范围 [first, last) 中的所有元素，从首元素开始逐次到末元素。
	//若 d_first 在范围 [first, last) 中则行为未定义。此情况下可用 copy_backward 代替。
	template< typename InputIt, typename OutputIt >
	inline OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = *first++;
		}
		return d_first;
	}

	//复制来自 [first, last) 所定义范围的元素，到终于 d_last 的范围。以逆序复制元素（首先复制末元素），但保持其相对顺序。
	//若 d_last 在(first, last] 中则行为未定义。该情况下必须用 std::copy 取代 std::copy_backward 。
	template< class BidirIt1, class BidirIt2 >
	BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
		while (first != last) {
			*(--d_last) = *(--last);
		}
		return d_last;
	}

	//若 count>0 ，则准确复制来自始于 first 的范围的 count 个值到始于 result 的范围。
	//不同于 copy ，本算法容许重叠。如果范围重叠，则结果指向的范围中的某些元素可能具有未定义但有效的值。
	template< typename InputIt, typename Size, typename OutputIt >
	inline OutputIt copy_n(InputIt first, Size count, OutputIt result) {
		for (; count > 0; --count) {
			*result++ = *first++;
		}
		return result;
	}

	// 赋值给定的 value 给 [first, last) 中的元素。
	template< typename ForwardIt, typename T >
	inline void fill(ForwardIt first, ForwardIt last, const T& value) {
		while (first != last) {
			*first++ = value;
		}
	}

	//若 count > 0 ，则赋值给定的 value 给始于 的范围的首 count 个元素。否则不做任何事。
	template< typename OutputIt, typename Size, typename T >
	inline OutputIt fill_n(OutputIt first, Size count, const T& value) {
		for (Size i = 0; i < count; i++) {
			*first++ = value;
		}
		return first;
	}

	//移动范围 [first, last) 中的元素到始于 d_first 的另一范围，从首元素开始逐次到末元素。
	//此操作后被移动范围中的元素将仍然含有适合类型的合法值，但不必与移动前的值相同。
	//若 d_first 在范围 [first, last) 中则行为未定义。此情况下可用 std::move_backward 代替 std::move 。
	template< typename InputIt, typename OutputIt >
	inline OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = std::move(*first++);
		}
		return d_first;
	}

	//移动来自范围 [first, last) 的元素到终于 d_last 的另一范围。以逆序移动元素（首先复制末元素），但保持其相对顺序。
	//若 d_last 在(first, last] 内则行为未定义。该情况下必须用 std::move 代替 std::move_backward 。
	template< typename BidirIt1, typename BidirIt2 >
	inline BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
		while (first != last) {
			*(--d_last) = std::move(*(--last));
		}
		return d_last;
	}

	//进行元素范围上的左旋转。
	//具体而言， rotate 交换范围[first, last) 中的元素，方式满足元素 n_first 成为新范围的首个元素，而 n_first - 1 成为最后元素。
	//此函数的前提条件是[first, n_first) 和[n_first, last) 为合法范围。
	//返回指向 first 指向的元素所在的新位置的迭代器。等于 first + (last - n_first)
	template< typename ForwardIt >
	ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last) {
		//if (first == n_first) return first;
		//if (n_first == last) return first;

		//auto right = n_first; 
		//auto left = first; 
		//auto next_n_first = n_first;
		//while (right != last) {
		//	if (left == next_n_first) next_n_first = right;
		//	mystl::swap(*left++, *right++);
		//}

		//// 旋转剩余序列到位置中
		//mystl::rotate(left, next_n_first, last);
		//return left;

		ForwardIt right = n_first;
		ForwardIt ret = first;
		bool ret_set = false;
		while (first != right)
		{
			mystl::swap(*first++, *right++);
			if (right == last) { 
				//右侧较短，此时原来[n_first,last)元素已经全部移到前面去了
				//但是[first,n_first)的元素应该在末尾,继续左旋
				right = n_first;
				if (!ret_set) {//第一次right == last时，first指向的位置为返回值
					ret = first;
					ret_set = true;
				}
			}
			else if (first == n_first) {//左侧较短，设置下一个n_first的位置
				n_first = right;
			}
		}
		return ret;
	}

	//检查第一个范围 [first1, last1) 是否按字典序小于第二个范围 [first2, last2) 

	//字典序比较是拥有下列属性的操作：
	// * 逐元素比较二个范围。
	// * 首个不匹配元素定义范围是否按字典序小于或大于另一个。
	// * 若一个范围是另一个的前缀，则较短的范围小于另一个。
	// * 若二个范围拥有等价元素和相同长度，则范围按字典序相等。
	// * 空范围按字典序小于任何非空范围。
	// * 二个空范围按字典序相等。
	template< class InputIt1, class InputIt2 >
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2) {
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	//能用于forward_iterator的归并排序
	template<typename ForwardIt, typename Compare>
	void __merge_sort(ForwardIt f1, ForwardIt l1,
		ForwardIt f2, ForwardIt l2,size_t n, Compare& comp) {
		if (n <= 1)
			return;
		size_t m = n / 2;
		ForwardIt low1 = f2, high1 = f2;
		mystl::advance(high1, m);
		ForwardIt low2 = high1, high2 = l2;

		ForwardIt low3 = f1, high3 = f1;
		mystl::advance(high3, m);
		ForwardIt low4 = high3, high4 = l1;
		
		__merge_sort(low1, high1, low3, high3, m,comp);
		__merge_sort(low2, high2, low4, high4, n - m, comp);
		while (low1 != high1 && low2 != high2) {
			*f1++ = comp(*low1,*low2) ? *low1++ : *low2++;
		}
		while (low1 != high1) {
			*f1++ = *low1++;
		}
		while (low2 != high2) {
			*f1++ = *low2++;
		}
	}

	template<typename Continer,typename ForwardIt, typename Compare = std::less<>>
	inline void merge_sort(ForwardIt first, ForwardIt last, Compare comp = Compare()) {
		Continer tmp(first, last);
		__merge_sort(first, last, tmp.begin(), tmp.end(), tmp.size(),comp);
	}


	//适合random_access_iterator的归并排序
	//low,high为有效下标
	template<typename Continer,typename Compare>
	void __merge_sort(Continer& cont, Continer& tmp, size_t low, size_t high, Compare comp) {
		if (low >= high) return;
		size_t mid = low + ((high - low) >> 1);
		size_t low1 = low, high1 = mid;
		size_t low2 = mid + 1, high2 = high;
		__merge_sort(tmp, cont, low1, high1, comp); //用tmp保存区域排序结果
		__merge_sort(tmp, cont, low2, high2, comp);

		size_t index = low1;
		while (low1 <= high1 && low2 <= high2) {
			cont[index++] = comp(tmp[low1], tmp[low2]) ? tmp[low1++] : tmp[low2++];
		}
		while (low1 <= high1) {
			cont[index++] = tmp[low1++];
		}
		while (low2 <= high2) {
			cont[index++] = tmp[low2++];
		}
	}

	template<typename Continer,typename Compare = std::less<>>
	void merge_sort(Continer& cont, Compare comp = Compare()) {
		Continer tmp = cont;
		__merge_sort(cont, tmp, 0, tmp.size() - 1,comp);

	}

	/***************************************************************************
	*				                 hash                                      *
	***************************************************************************/
	template<typename T,typename = void>
	struct hash{};

	//bool, char, signed char, unsigned char,char8_t,wchar_t,char16_t, char32_t,
	//short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long
	//枚举类型
	template<typename T>
	struct hash<T,std::void_t<std::is_integral<T>,std::is_enum<T>,std::is_pointer<T>>>
	{
		unsigned operator()(T value) const {
			return static_cast<unsigned>(value);
		}
	};

	//指针类型
	template<typename T>
	struct hash<T*>
	{
		unsigned operator()(T* ptr) const {
			unsigned ret = 0;
			memmove(&ret, ptr, sizeof(unsigned));
			return ret;
		}
	};


	/***************************************************************************
	*				                heap 操作                                  *
	***************************************************************************/

	
	//下沉操作
	template<typename RandomIt, typename Compare,typename Distance>
	void adjust_down(RandomIt first,Distance parent_index, Distance size, Compare comp) {
		using T = iter_value_t<RandomIt>;
		T tmp = *(first + parent_index); //暂存需要下沉的值

		Distance child_index = 2 * parent_index + 1;
		
		while (child_index < size) {
			//选择权值更大的子节点
			if (child_index + 1 < size && comp(*(first + child_index), *(first + child_index + 1))) {
				++child_index;
			}
			//如果需要下沉的值的权值“不小于”权值更大的子节点，下沉结束
			if (!comp(tmp,*(first + child_index)))
				break;

			*(first + parent_index) = *(first + child_index);
			parent_index = child_index;
			child_index = 2 * parent_index + 1;
		}
		*(first + parent_index) = tmp;
	}

	//上浮操作
	template< typename RandomIt, typename Compare >
	void adjust_up(RandomIt first, RandomIt last, Compare comp) 
	{
		using Distance = iter_diff_t<RandomIt>;
		using T = iter_value_t<RandomIt>;

		Distance child_index = (last - first) - 1;
		Distance parent_index = (child_index - 1) / 2;

		T tmp = *(first + child_index);//暂存需要上浮的值

		//没有到达顶端，且需要上浮的值的权值“大于”父节点的权值
		while (child_index > 0 && comp(*(first + parent_index), tmp)) {
			*(first + child_index) = *(first + parent_index);
			child_index = parent_index;
			parent_index = (child_index - 1) / 2;
		}
		*(first + child_index) = tmp;
	}

	//在范围[first, last) 中构造最大堆。
	template< typename RandomIt, typename Compare >
	inline void make_heap(RandomIt first, RandomIt last, Compare comp)
	{
		using Distance = iter_diff_t<RandomIt>;
		Distance size = last - first;

		if (size <= 1)
			return;
		//非叶子节点的下标
		Distance non_leaf_index = size / 2 - 1;
		while (non_leaf_index >= 0) {
			adjust_down(first, non_leaf_index, size, comp);
			--non_leaf_index;
		}
	}

	template< typename RandomIt >
	inline void make_heap(RandomIt first, RandomIt last)
	{
		mystl::make_heap(first, last, std::less<iter_value_t<RandomIt>>());
	}

	//插入位于位置 last-1 的元素到范围 [first, last-1) 所定义的最大堆中
	template< typename RandomIt, typename Compare >
	inline void push_heap(RandomIt first, RandomIt last, Compare comp)
	{
		adjust_up(first, last, comp);
	}

	template< typename RandomIt >
	inline void push_heap(RandomIt first, RandomIt last)
	{
		mystl::push_heap(first, last, std::less<iter_value_t<RandomIt>>());
	}

	//交换在位置 first 的值和在位置 last-1 的值，并令子范围 [first, last-1) 变为堆
	//这拥有从范围 [first, last) 所定义的堆移除首个元素的效果
	template< typename RandomIt, typename Compare >
	inline void pop_heap(RandomIt first, RandomIt last, Compare comp)
	{
		using Distance = iter_diff_t<RandomIt>;
		//交换堆尾堆顶元素
		mystl::swap(*first, *(last - 1));
		//在[first,last - 1)范围内下沉堆顶元素
		adjust_down(first, Distance(0), last - first - 1, comp);
	}

	template< typename RandomIt >
	inline void pop_heap(RandomIt first, RandomIt last)
	{
		mystl::pop_heap(first, last, std::less<iter_value_t<RandomIt>>());
	}

	//转换最大堆 [first, last) 为以升序排序的范围。产生的范围不再拥有堆属性
	template< typename RandomIt, typename Compare >
	inline void sort_heap(RandomIt first, RandomIt last, Compare comp)
	{
		while (first != last)
			mystl::pop_heap(first, last--, comp);
	}

	template< typename RandomIt >
	inline void sort_heap(RandomIt first, RandomIt last)
	{
		mystl::sort_heap(first, last, std::less<iter_value_t<RandomIt>>());
	}

	
}//end of namespace mystl

#endif // !_MY_ALGORITHM_H_
