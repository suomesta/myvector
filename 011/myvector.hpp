/**
 * @file    myvector.hpp
 * @brief   Implementation of myvector class. 
 *          myvector class imitates of std::vector. 
 * @author  toda
 * @date    2019-05-10
 * @version 0.1.0
 * @remark  the target is C++11 or more.
 */

#ifndef INCLUDE_GUARD_MYVECTOR_HPP
#define INCLUDE_GUARD_MYVECTOR_HPP

#include <cstddef>
#include <iterator>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

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

private:
    enum : size_type {
        MAX_SIZE_BYTES = std::numeric_limits<size_type>::max(),
        MAX_SIZE = MAX_SIZE_BYTES / sizeof(value_type),
    };
    using self_type = myvector<value_type>;
    /////////////////////////////////////////////////////////////////////////////

public:
    /**
     * @brief      Default constructor. Constructs an empty container.
     */
    myvector(void) noexcept = default;
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Constructor with size and initial value. CConstructs the container
     *             with count copies of elements with value.
     * @param[in]  count: The size of the container.
     * @param[in]  value: The value to initialize elements of the container with.
     * @throw      std::length_error: If count is greater than max_size().
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    myvector(size_type count, const value_type& value) :
        heap_(count ? mymalloc(length_check(count)) : nullptr),
        size_(count),
        capacity_(count)
    {
        for (size_type i = 0; i < size_; i++) {
            new(&heap_[i]) value_type(value);
        }
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Constructor with size. Constructs the container with count
     *             default-inserted instances of T.
     * @param[in]  count: The size of the container.
     * @throw      std::length_error: If count is greater than max_size().
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    explicit myvector(size_type count) :
        heap_(count ? mymalloc(length_check(count)) : nullptr),
        size_(count),
        capacity_(count)
    {
        for (size_type i = 0; i < size_; i++) {
            new(&heap_[i]) value_type();
        }
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Copy constructor. Constructs the container with the copy of the
     *             contents of other.
     * @param[in,out] other: Another container to be used as source to initialize the
     *                       elements of the container with.
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    myvector(const self_type& other) :
        heap_(other.size() ? mymalloc(other.size()) : nullptr),
        size_(other.size()),
        capacity_(other.size())
    {
        pointer p = heap_;
        for (auto i = other.cbegin(); i != other.cend(); ++i) {
            new(p++) value_type(*i);
        }
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Move constructor. Constructs the container with the contents of
     *             other using move semantics. After the move, other is guaranteed
     *             to be empty().
     * @param[in,out] other: Another container to be used as source to initialize the
     *                       elements of the container with.
     */
    myvector(self_type&& other) noexcept
    {
        std::swap(heap_, other.heap_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Destructor. Destructs the vector.
     */
    ~myvector(void)
    {
        for (size_type i = 0; i < size_; i++) {
            heap_[i].~value_type();
        }
        free(heap_);
        heap_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
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
     * @brief      Returns the maximum number of elements the container is able to
     *             hold due to system or library implementation limitations, i.e.
     *             std::distance(begin(), end()) for the largest container.
     * @return     Maximum number of elements.
     */
    size_type max_size(void) const noexcept
    {
        return MAX_SIZE;
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
    /**
     * @brief      Checks new container size.
     * @param[in]  count: Number of elements.
     * @return     Same as count.
     * @throw      std::length_error: If count is greater than the maximum size.
     */
    size_type length_check(size_type count) const
    {
        if (count > MAX_SIZE) {
            throw std::length_error("myvecotr::length_check()");
        }

        return count;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Calls malloc with (sizeof(value_type) * count).
     * @param[in]  count: Number of elements. Should be count > 0.
     * @return     A pointer to the memory block allocated by malloc().
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    pointer mymalloc(size_type count)
    {
        void* p = malloc(sizeof(value_type) * count);
        if (p == nullptr) {
            throw std::bad_alloc();
        }

        return static_cast<pointer>(p);
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
