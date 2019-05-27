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

    /**
     * @brief      Returns an iterator to the first element of the container.
     *             If the container is empty, the returned iterator will be equal
     *             to end().
     * @return     Iterator to the first element.
     */
    iterator begin(void) noexcept
    {
        return heap_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a const iterator to the first element of the container.
     *             If the container is empty, the returned iterator will be equal
     *             to end().
     * @return     Const iterator to the first element.
     */
    const_iterator begin(void) const noexcept
    {
        return heap_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a const iterator to the first element of the container.
     *             If the container is empty, the returned iterator will be equal
     *             to cend().
     * @return     Const iterator to the first element.
     */
    const_iterator cbegin(void) const noexcept
    {
        return heap_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns an iterator to the element following the last element
     *             of the container.
     *             This element acts as a placeholder; attempting to access it
     *             results in undefined behavior.
     * @return     Iterator to the element following the last element.
     */
    iterator end(void) noexcept
    {
        return heap_ + size_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a const iterator to the element following the last element
     *             of the container.
     *             This element acts as a placeholder; attempting to access it
     *             results in undefined behavior.
     * @return     Const iterator to the element following the last element.
     */
    const_iterator end(void) const noexcept
    {
        return heap_ + size_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a const iterator to the element following the last element
     *             of the container.
     *             This element acts as a placeholder; attempting to access it
     *             results in undefined behavior.
     * @return     Const iterator to the element following the last element.
     */
    const_iterator cend(void) const noexcept
    {
        return heap_ + size_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns the number of elements in the container.
     * @return     The number of elements in the container.
     */
    size_type size(void) const noexcept
    {
        return size_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns the number of elements that the container has currently
     *             allocated space for.
     * @return     Capacity of the currently allocated storage.
     */
    size_type capacity(void) const noexcept
    {
        return capacity_;
    }
    /////////////////////////////////////////////////////////////////////////////

private:
    pointer heap_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
    /////////////////////////////////////////////////////////////////////////////
};
/////////////////////////////////////////////////////////////////////////////

#endif // #ifndef INCLUDE_GUARD_MYVECTOR_HPP
