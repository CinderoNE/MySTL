#pragma once
#ifndef _MY_TYPE_TRAITS_H_
#define _MY_TYPE_TRAITS_H_

#include<type_traits> 
#include<utility> //for std::pair
namespace mystl {
	template <class _Ty, _Ty _Val>
	struct integral_constant {
		static constexpr _Ty value = _Val;

		using value_type = _Ty;
		using type = integral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}

		constexpr value_type operator()() const noexcept {
			return value;
		}
	};
	template<bool b>
	using bool_constant = integral_constant<bool, b>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	//能否从From转换为To

	template <typename From, typename To>
	constexpr bool is_convertible_v = std::is_convertible<From, To>::value; //std

	//是否是不重要的拷贝赋值
	template<typename T>
	constexpr bool is_trivially_copy_assignable_v = std::is_trivially_copy_assignable<T>::value;

	template<typename T>
	struct is_trivially_copy_assignable : bool_constant<is_trivially_copy_assignable_v<T>>{};

	//是否是不重要的移动赋值
	template<typename T>
	constexpr bool is_trivially_move_assignable_v = std::is_trivially_move_assignable<T>::value;

	template<typename T>
	struct is_trivially_move_assignable : bool_constant<is_trivially_move_assignable_v<T>> {};


	//根据类型T判断是否是pair
	template<typename T>
	constexpr bool is_pair_v = false;

	template<typename T1, typename T2>
	constexpr bool is_pair_v<std::pair<T1, T2>> = true;
	
} //end of namespace mystl

#endif // !_MY_TYPE_TRAITS_H_
