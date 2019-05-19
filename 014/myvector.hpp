/**
 * @file    myvector.hpp
 * @brief   Implementation of myvector class. 
 *          myvector class imitates of std::vector. 
 * @author  toda
 * @date    2019-05-19
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
#include <type_traits>
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

    struct trivially_copyable_tag {};
    struct move_constructible_tag {};
    struct copy_constructible_tag {};
    using realloc_switcher = typename std::conditional<
        std::is_trivially_copyable<value_type>::value,
        trivially_copyable_tag,
        typename std::conditional<
            std::is_move_constructible<value_type>::value,
            move_constructible_tag,
            copy_constructible_tag
        >::type
    >::type;
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
     * @brief      Constructs the container with the contents of the range [first, last).
     * @param[in]  first: First of the range to copy the elements from.
     * @param[in]  last: Last of the range to copy the elements from.
     * @throw      std::length_error: If distance between first and last is greater than max_size().
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     * @tparam     InputIt: Iterator to the elements.
     */
    template <class InputIt>
    myvector(InputIt first,
             typename std::enable_if<not std::is_integral<InputIt>::value, InputIt>::type last)
    {
        // Switcher for mydistance(). Whether InputIt is random access iterator or input iterator.
        using iterator_type = typename std::iterator_traits<InputIt>::iterator_category;

        size_type count = mydistance(first, last, iterator_type());
        if (count) {
            heap_ = mymalloc(length_check(count));
            size_ = count;
            capacity_ = count;

            pointer p = heap_;
            for (InputIt i = first; i != last; ++i) {
                new(p++) value_type(*i);
            }
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
     * @brief      Constructs the container with the contents of the initializer list.
     * @param[in]  init: Initializer list to initialize the elements of the container with.
     * @throw      std::length_error: If distance between first and last is greater than max_size().
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    myvector(std::initializer_list<value_type> init) :
        heap_(init.size() ? mymalloc(length_check(init.size())) : nullptr),
        size_(init.size()),
        capacity_(init.size())
    {
        pointer p = heap_;
        for (const auto& i: init) {
            new(p++) value_type(i);
        }
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
     * @brief      Increase the capacity of the vector to a value that's greater or
     *             equal to new_cap. If new_cap is greater than the current capacity(),
     *             new storage is allocated, otherwise the method does nothing.
     *             reserve() does not change the size of the vector.
     *             If new_cap is greater than capacity(), all iterators, including the
     *             past-the-end iterator, and all references to the elements are
     *             invalidated. Otherwise, no iterators or references are invalidated.
     * @param[in]  new_cap: New capacity of the vector.
     */
    void reserve(size_type new_cap)
    {
        if (new_cap > capacity_) { // expand capacity
            myrealloc(length_check(new_cap), realloc_switcher());
        }
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
     * @brief      Counts distance from first to last.
     *             The return type is size_type (not difference_type).
     * @param[in]  first: First of the range.
     * @param[in]  last: Last of the range.
     * @param[in]  std::random_access_iterator_tag: Function switcher according to iterator type.
     * @return     Distance from first to last in size_type.
     * @tparam     RandomIt: Random access iterator of the range.
     */
    template <class RandomIt>
    size_type mydistance(RandomIt first, RandomIt last, std::random_access_iterator_tag) const
    {
        if ((first <= last) && (last - first >= 0)) { // confirm never overflows
            return last - first;
        }
        else {
            return mydistance(first, last, std::input_iterator_tag());
        }
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Counts distance from first to last.
     *             The return type is size_type (not difference_type).
     * @param[in]  first: First of the range.
     * @param[in]  last: Last of the range.
     * @param[in]  std::input_iterator_tag: Function switcher according to iterator type.
     * @return     Distance from first to last in size_type.
     * @tparam     InputIt: Input iterator of the range.
     */
    template <class InputIt>
    size_type mydistance(InputIt first, InputIt last, std::input_iterator_tag) const
    {
        size_type count = 0;
        for (InputIt i = first; i != last; ++i) {
            count++;
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

    /**
     * @brief      Does reallocation by calling reaclloc().
     *             value_type shall be TriviallyCopyable.
     * @param[in]  new_cap: New capacity of the vector. Should be new_cap > 0.
     * @param[in]  trivially_copyable_tag: Function switcher according to value_type characteristic.
     * @throw      std::bad_alloc: If realloc() fails to allocate storage.
     */
    void myrealloc(size_type new_cap, trivially_copyable_tag)
    {
        // new allocation
        void* p = realloc(static_cast<void*>(heap_), sizeof(value_type) * new_cap);
        if (p == nullptr) {
            throw std::bad_alloc();
        }

        // set new values to member variables
        if (static_cast<pointer>(p) != heap_) {
            heap_ = static_cast<pointer>(p);
        }
        capacity_ = new_cap;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Does reallocation by calling malloc().
     *             value_type shall be MoveConstructible.
     * @param[in]  new_cap: New capacity of the vector. Should be new_cap > 0.
     * @param[in]  move_constructible_tag: Function switcher according to value_type characteristic.
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    void myrealloc(size_type new_cap, move_constructible_tag)
    {
        // new allocation
        pointer p = mymalloc(new_cap); // may throw std::bad_alloc

        // copy to new address by move-constructor, and clean up
        for (size_type i = 0; i < size_; i++) {
            new(&p[i]) value_type(std::move(heap_[i]));
            heap_[i].~value_type();
        }
        free(heap_);

        // set new values to member variables
        heap_ = p;
        capacity_ = new_cap;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Does reallocation by calling malloc().
     *             value_type shall be CopyConstructible.
     * @param[in]  new_cap: New capacity of the vector. Should be new_cap > 0.
     * @param[in]  copy_constructible_tag: Function switcher according to value_type characteristic.
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    void myrealloc(size_type new_cap, copy_constructible_tag)
    {
        // new allocation
        pointer p = mymalloc(new_cap); // may throw std::bad_alloc

        // copy to new address by copy-constructor, and clean up
        for (size_type i = 0; i < size_; i++) {
            new(&p[i]) value_type(heap_[i]);
            heap_[i].~value_type();
        }
        free(heap_);

        // set new values to member variables
        heap_ = p;
        capacity_ = new_cap;
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