#pragma once

#include"my_construct.h"
#include"my_type_traits.h"
#include"my_iterator.h"
#include"my_algorithm.h"

namespace mystl {
   

    /*uninitialized_copy*/

    //trivially_copy_assignable
    template< typename InputIt, typename ForwardIt >
    inline ForwardIt 
    __uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first,mystl::true_type) {
        return mystl::copy(first, last, d_first);
    }

    //non trivially_copy_assignable
    template< typename InputIt, typename ForwardIt >
    inline ForwardIt 
    __uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first, mystl::false_type) {
        ForwardIt current = d_first;
        try {
            for (; first != last; ++first, ++current) {
                mystl::construct(std::addressof(*current), *first);
            }
            return current;
        }
        catch (...) {
            mystl::destroy(d_first, current);
            throw;
        }
    }

    template <typename InputIt, typename ForwardIt>
    inline ForwardIt 
    uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first) {
        return __uninitialized_copy(first, last, d_first, 
            mystl::is_trivially_copy_assignable<mystl::iter_value_t<ForwardIt>>{});
    }

    /*uninitialized_copy_n*/

    //trivially_copy_assignable
    template< class InputIt, class Size, class ForwardIt >
    ForwardIt __uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first, mystl::true_type) {
        return mystl::copy_n(first, count, d_first);
    }

    //non trivially_copy_assignable
    template< class InputIt, class Size, class ForwardIt >
    ForwardIt __uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first, mystl::false_type) {
        ForwardIt current = d_first;
        try {
            for (; count > 0; ++first, ++current,--count) {
                mystl::construct(std::addressof(*current), *first);
            }
            return current;
        }
        catch (...) {
            mystl::destroy(d_first, current);
            throw;
        }
    }

    template< class InputIt, class Size, class ForwardIt >
    ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first) {
        return __uninitialized_copy_n(first, count, d_first,
            mystl::is_trivially_copy_assignable<mystl::iter_value_t<ForwardIt>>{});
    }

   /* uninitialized_fill*/

    template< class ForwardIt, class T >
    void __uninitialized_fill(ForwardIt first, ForwardIt last, const T& value, mystl::true_type) {
        mystl::fill(first, last, value);
    }

    template< class ForwardIt, class T >
    void __uninitialized_fill(ForwardIt first, ForwardIt last, const T& value, mystl::false_type) {
        ForwardIt current = first;
        try {
            for (; current != last; ++current) {
                mystl::construct(std::addressof(*current), *first);
            }
        }
        catch (...) {
            mystl::destroy(first, current);
            throw;
        }
    }

    template< class ForwardIt, class T >
    void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value) {
        __uninitialized_fill(first, last, value,
            mystl::is_trivially_copy_assignable< mystl::iter_value_t<ForwardIt>>{});
    }

    /*uninitialized_fill_n*/

    template< class ForwardIt, class Size, class T >
    ForwardIt __uninitialized_fill_n(ForwardIt first, Size count, const T& value, mystl::true_type) {
        return mystl::fill_n(first, count, value);
    }

    template< class ForwardIt, class Size, class T >
    ForwardIt __uninitialized_fill_n(ForwardIt first, Size count, const T& value, mystl::false_type) {
        ForwardIt current = first;
        try {
            for (; count > 0; ++current, --count) {
                mystl::construct(std::addressof(*current), *first);
            }
            return current;
        }
        catch (...) {
            mystl::destroy(first, current);
            throw;
        }
    }

    template< class ForwardIt, class Size, class T >
    ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value) {
        return __uninitialized_fill_n(first, count, value,
            mystl::is_trivially_copy_assignable<mystl::iter_value_t<ForwardIt>>{});
    }


    /*uninitialized_move*/

    template< class InputIt, class ForwardIt >
    ForwardIt __uninitialized_move(InputIt first, InputIt last, ForwardIt d_first, mystl::true_type) {
        return mystl::move(first, last, d_first);
    }

    template< class InputIt, class ForwardIt >
    ForwardIt __uninitialized_move(InputIt first, InputIt last, ForwardIt d_first, mystl::false_type) {
        ForwardIt current = d_first;
        try {
            for (; first != last; ++first, ++current) {
                mystl::construct(std::addressof(*current), std::move(*first));
            }
            return current;
        }
        catch (...) {
          
            mystl::destroy(d_first, current);
            throw;
        }
    }

    template< class InputIt, class ForwardIt >
    ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt d_first) {
        return __uninitialized_move(first, last, d_first,
            mystl::is_trivially_move_assignable< mystl::iter_value_t<ForwardIt>>{});
    }

  
} //end of namespace mystl