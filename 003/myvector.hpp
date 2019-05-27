/**
 * @file    myvector.hpp
 * @brief   Implementation of myvector class.
 *          myvector class imitates std::vector.
 * @author  toda
 * @date    2019-05-06
 * @version 0.1.0
 * @remark  the target is C++11 or more.
 */

#ifndef INCLUDE_GUARD_MYVECTOR_HPP
#define INCLUDE_GUARD_MYVECTOR_HPP

#include <cstddef>
#include <iterator>

/**
 * @class   myvector
 * @brief   myvector is a class which imitates std::vector.
 * @tparam  T: The type of the elements.
 */
template <typename T>
class myvector
{
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    /////////////////////////////////////////////////////////////////////////////

public:
    /**
     * @brief      Default constructor. Constructs an empty container.
     */
    myvector(void) noexcept = default;
    /////////////////////////////////////////////////////////////////////////////

private:
    pointer heap_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
    /////////////////////////////////////////////////////////////////////////////
};
/////////////////////////////////////////////////////////////////////////////

#endif // #ifndef INCLUDE_GUARD_MYVECTOR_HPP
