/**
 * @file    myvector.hpp
 * @brief   Implementation of myvector class.
 *          myvector class imitates std::vector.
 * @author  toda
 * @date    2019-05-26
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
     * @brief      Constructor with size and initial value. Constructs the container
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
     * @brief      Copy assignment operator. Replaces the contents with a copy of the
     *             contents of other.
     * @param[in]  other: Another container to use as data source.
     * @return     Own reference.
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    self_type& operator=(const self_type& other)
    {
        if (this != &other) {
            // clear
            for (size_type i = 0; i < size_; i++) {
                heap_[i].~value_type();
            }
            size_ = 0;

            // re-allocate if needed
            if (other.size() > capacity_) {
                reallocation(other.size(), realloc_switcher());
            }

            // assign
            pointer p = heap_;
            for (auto i = other.cbegin(); i != other.cend(); ++i) {
                new(p++) value_type(*i);
            }
            size_ = other.size();
        }
        return *this;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Move assignment operator. Replaces the contents with those of
     *             other using move semantics (i.e. the data in other is moved from
     *             other into this container). other is in a valid but unspecified
     *             state afterwards. In any case, all elements originally present
     *             in *this are either destroyed or replaced by element-wise move-assignment.
     * @param[in]  other: Another container to use as data source.
     * @return     Own reference.
     */
    self_type& operator=(self_type&& other) noexcept
    {
        if (this != &other) {
            // clear
            for (size_type i = 0; i < size_; i++) {
                heap_[i].~value_type();
            }
            size_ = 0;

            // swap
            std::swap(heap_, other.heap_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
        }
        return *this;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Replaces the contents with those identified by initializer list ilist.
     * @param[in]  ilist: Initializer list to use as data source.
     * @return     Own reference.
     * @throw      std::length_error: If ilist size is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    self_type& operator=(std::initializer_list<value_type> ilist)
    {
        size_type new_size = length_check(ilist.size());

        // clear
        for (size_type i = 0; i < size_; i++) {
            heap_[i].~value_type();
        }
        size_ = 0;

        // re-allocate if needed
        if (new_size > capacity_) {
            reallocation(new_size, realloc_switcher());
        }

        // assign
        pointer p = heap_;
        for (const auto& i: ilist) {
            new(p++) value_type(i);
        }
        size_ = ilist.size();

        return *this;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Replaces the contents with count copies of value.
     * @param[in]  count: The new size of the container.
     * @param[in]  value: The value to initialize elements of the container with.
     * @throw      std::length_error: If count is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void assign(size_type count, const value_type& value)
    {
        size_type new_size = length_check(count);

        // clear
        for (size_type i = 0; i < size_; i++) {
            heap_[i].~value_type();
        }
        size_ = 0;

        // re-allocate if needed
        if (new_size > capacity_) {
            reallocation(new_size, realloc_switcher());
        }

        // assign
        pointer p = heap_;
        for (size_type i = 0; i < new_size; i++) {
            new(p++) value_type(value);
        }
        size_ = new_size;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Replaces the contents with copies of those in the range [first, last).
     * @param[in]  first: The first of the range to copy the elements from.
     * @param[in]  last: The last of the range to copy the elements from.
     * @throw      std::length_error: If distance between first and last is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     * @tparam     InputIt: Iterator to the elements.
     */
    template <class InputIt>
    void assign(InputIt first,
                typename std::enable_if<not std::is_integral<InputIt>::value, InputIt>::type last)
    {
        // Switcher for mydistance(). Whether InputIt is random access iterator or input iterator.
        using iterator_type = typename std::iterator_traits<InputIt>::iterator_category;

        size_type new_size = length_check(mydistance(first, last, iterator_type()));

        // clear
        for (size_type i = 0; i < size_; i++) {
            heap_[i].~value_type();
        }
        size_ = 0;

        // re-allocate if needed
        if (new_size > capacity_) {
            reallocation(new_size, realloc_switcher());
        }

        // assign
        pointer p = heap_;
        for (InputIt i = first; i != last; ++i) {
            new(p++) value_type(*i);
        }
        size_ = new_size;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Replaces the contents with the elements from the initializer list ilist.
     * @param[in]  ilist: Initializer list to copy the values from.
     * @throw      std::length_error: If ilist size is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void assign(std::initializer_list<value_type> ilist)
    {
        size_type new_size = length_check(ilist.size());

        // clear
        for (size_type i = 0; i < size_; i++) {
            heap_[i].~value_type();
        }
        size_ = 0;

        // re-allocate if needed
        if (new_size > capacity_) {
            reallocation(new_size, realloc_switcher());
        }

        // assign
        pointer p = heap_;
        for (const auto& i: ilist) {
            new(p++) value_type(i);
        }
        size_ = new_size;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a reference to the element at specified location pos,
     *             with bounds checking. If pos is not within the range of the container,
     *             an exception of type std::out_of_range is thrown.
     * @param[in]  pos: Position of the element to return.
     * @return     Reference to the requested element.
     * @throw      std::out_of_range: If pos is not within the range of the container.
     */
    reference at(size_type pos)
    {
        if (pos >= size_) {
            throw std::out_of_range("myvector::at()");
        }
        return heap_[pos];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a reference to the element at specified location pos,
     *             with bounds checking. If pos is not within the range of the container,
     *             an exception of type std::out_of_range is thrown.
     * @param[in]  pos: Position of the element to return.
     * @return     Const reference to the requested element.
     * @throw      std::out_of_range: If pos is not within the range of the container.
     */
    const_reference at(size_type pos) const
    {
        if (pos >= size_) {
            throw std::out_of_range("myvector::at() const");
        }
        return heap_[pos];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a reference to the element at specified location pos.
     *             No bounds checking is performed.
     * @param[in]  pos: Position of the element to return.
     * @return     Reference to the requested element.
     */
    reference operator[](size_type pos)
    {
        return heap_[pos];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a reference to the element at specified location pos.
     *             No bounds checking is performed.
     * @param[in]  pos: Position of the element to return.
     * @return     Const reference to the requested element.
     */
    const_reference operator[](size_type pos) const
    {
        return heap_[pos];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a reference to the first element in the container.
     *             Calling front on an empty container is undefined.
     * @return     Reference to the first element
     */
    reference front(void)
    {
        return heap_[0];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns a reference to the first element in the container.
     *             Calling front on an empty container is undefined.
     * @return     Const reference to the first element
     */
    const_reference front(void) const
    {
        return heap_[0];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns reference to the last element in the container.
     *             Calling back on an empty container is undefined.
     * @return     Reference to the last element.
     */
    reference back(void)
    {
        return heap_[size_ - 1];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns reference to the last element in the container.
     *             Calling back on an empty container is undefined.
     * @return     Const reference to the last element.
     */
    const_reference back(void) const
    {
        return heap_[size_ - 1];
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns pointer to the underlying array serving as element storage.
     *             The pointer is such that range [data(), data() + size()) is always
     *             a valid range, even if the container is empty (data() is not dereferenceable
     *             in that case).
     * @return     Pointer to the underlying element storage. For non-empty containers,
     *             the returned pointer compares equal to the address of the first element.
     */
    pointer data(void) noexcept
    {
        return heap_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Returns pointer to the underlying array serving as element storage.
     *             The pointer is such that range [data(), data() + size()) is always
     *             a valid range, even if the container is empty (data() is not dereferenceable
     *             in that case).
     * @return     Const pointer to the underlying element storage. For non-empty containers,
     *             the returned pointer compares equal to the address of the first element.
     */
    const_pointer data(void) const noexcept
    {
        return heap_;
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
     * @brief      Checks if the container has no elements.
     * @return     true if the container is empty, false otherwise.
     */
    bool empty(void) const noexcept
    {
        return size_ == 0;
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
     * @throw      std::length_error: If new_cap is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void reserve(size_type new_cap)
    {
        if (new_cap > capacity_) { // expand capacity
            reallocation(length_check(new_cap), realloc_switcher());
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

    /**
     * @brief      Removal of unused capacity.
     */
    void shrink_to_fit(void)
    {
        if (size_ < capacity_) {
            if (size_ == 0) {
                free(heap_);
                heap_ = nullptr;
                capacity_ = 0;
            }
            else {
                reallocation(size_, realloc_switcher());
            }
        }
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Appends the given element value to the end of the container.
     *             The new element is initialized as a copy of value.
     *             If the new size() is greater than capacity() then all iterators
     *             and references (including the past-the-end iterator) are invalidated.
     *             Otherwise only the past-the-end iterator is invalidated.
     * @param[in]  value: The value of the element to append.
     * @throw      std::length_error: If new capacity is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void push_back(const value_type& value)
    {
        if (need_twice_capacity()) {
            reallocation(twice_length(), realloc_switcher());
        }
        new(&heap_[size_]) value_type(value);
        size_++;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Appends the given element value to the end of the container.
     *             The value is moved into the new element.
     *             If the new size() is greater than capacity() then all iterators
     *             and references (including the past-the-end iterator) are invalidated.
     *             Otherwise only the past-the-end iterator is invalidated.
     * @param[in,out] value: The value of the element to append.
     * @throw      std::length_error: If new capacity is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void push_back(value_type&& value)
    {
        if (need_twice_capacity()) {
            reallocation(twice_length(), realloc_switcher());
        }
        new(&heap_[size_]) value_type(std::forward<value_type&&>(value));
        size_++;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Resizes the container to contain count elements.
     *             If the current size is greater than count, the container is reduced
     *             to its first count elements.
     * @param[in]  count: New size of the container.
     * @throw      std::length_error: If count is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void resize(size_type count)
    {
        if (count < size_) { // reduce size
            // call destructor to elements to be removed
            for (size_type i = count; i < size_; i++) {
                heap_[i].~value_type();
            }
            size_ = count;
        }
        else if (count > size_) { // enlarge size
            if (count > capacity_) { // expand capacity if needed
                reallocation(length_check(count), realloc_switcher());
            }
            // call placement-new to the expanded elements
            for (size_type i = size_; i < count; i++) {
                new(&heap_[i]) value_type();
            }
            size_ = count;
        }
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Resizes the container to contain count elements.
     *             If the current size is greater than count, the container is reduced
     *             to its first count elements.
     * @param[in]  count: New size of the container.
     * @param[in]  value: The value to initialize the new elements with.
     * @throw      std::length_error: If count is greater than max_size().
     * @throw      std::bad_alloc: If malloc() (or realloc()) fails to allocate storage.
     */
    void resize(size_type count, const value_type& value)
    {
        if (count < size_) { // reduce size
            // call destructor to elements to be removed
            for (size_type i = count; i < size_; i++) {
                heap_[i].~value_type();
            }
            size_ = count;
        }
        else if (count > size_) { // enlarge size
            if (count > capacity_) { // expand capacity if needed
                reallocation(length_check(count), realloc_switcher());
            }
            // call placement-new to the expanded elements
            for (size_type i = size_; i < count; i++) {
                new(&heap_[i]) value_type(value);
            }
            size_ = count;
        }
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
     * @brief      Check twice capacity is needed or not.
     * @return     true: Need twice capacity.
     *             false: Not need twice capacity.
     * @throw      std::length_error: If size_ already reaches MAX_SIZE.
     */
    bool need_twice_capacity(void) const
    {
        if (size_ >= MAX_SIZE) {
            throw std::length_error("myvecotr::need_twice_capacity()");
        }
        return (size_ + 1) > capacity_;
    }
    /////////////////////////////////////////////////////////////////////////////

    /**
     * @brief      Make twice capacity value.
     * @return     New capacity which is twice as current capacity.
     *             If current capacity is 0, then return 1.
     * @throw      std::length_error: If new capacity is greater than the maximum size.
     */
    size_type twice_length(void) const
    {
        if (capacity_ > (MAX_SIZE / 2)) {
            throw std::length_error("myvecotr::twice_length()");
        }

        return capacity_ ? capacity_ * 2 : 1;
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
     * @brief      Calls malloc() with (sizeof(value_type) * count).
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
     * @param[in]  new_cap: New capacity of the vector. Should be new_cap >= size_.
     * @param[in]  trivially_copyable_tag: Function switcher according to value_type characteristic.
     * @throw      std::bad_alloc: If realloc() fails to allocate storage.
     */
    void reallocation(size_type new_cap, trivially_copyable_tag)
    {
        // reallocation by calling realloc()
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
     * @param[in]  new_cap: New capacity of the vector. Should be new_cap >= size_.
     * @param[in]  move_constructible_tag: Function switcher according to value_type characteristic.
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    void reallocation(size_type new_cap, move_constructible_tag)
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
     * @param[in]  new_cap: New capacity of the vector. Should be new_cap >= size_.
     * @param[in]  copy_constructible_tag: Function switcher according to value_type characteristic.
     * @throw      std::bad_alloc: If malloc() fails to allocate storage.
     */
    void reallocation(size_type new_cap, copy_constructible_tag)
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
