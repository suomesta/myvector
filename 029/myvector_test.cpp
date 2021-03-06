/**
 * @file    myvector_test.cpp
 * @brief   test of myvector using lest. 
 * @author  toda
 * @date    2019-07-23
 * @version 0.1.0
 * @remark  the target is C++11 or more.
 */

#include <list>
#include <type_traits>
#include <utility>
#include "../include/lest.hpp"
#include "myvector.hpp"

namespace {

// non-trivial and non-copyable struct
struct noncopyable
{
    noncopyable(void) = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable(noncopyable&&) = default;
    noncopyable& operator=(const noncopyable&) = delete;
    noncopyable& operator=(noncopyable&&) = default;
    virtual ~noncopyable(void) = default;
};

// non-trivial and unmovable struct
struct unmovable
{
    unmovable(void) = default;
    unmovable(const unmovable&) = default;
    unmovable(unmovable&&) = delete;
    unmovable& operator=(const unmovable&) = default;
    unmovable& operator=(unmovable&&) = delete;
    virtual ~unmovable(void) = default;
};

template <typename T>
struct is_const_lvalue_reference : std::conditional<std::is_lvalue_reference<T>::value && std::is_const<typename std::remove_reference<T>::type>::value,
                                                    std::true_type,
                                                    std::false_type>::type {};

template <typename T>
struct is_const_pointer : std::conditional<std::is_pointer<T>::value && std::is_const<typename std::remove_pointer<T>::type>::value,
                                           std::true_type,
                                           std::false_type>::type {};

} // namespace {

const lest::test specification[] =
{

    CASE("001: class definition")
    {
        // no actual tests
        myvector<char> cv;
        myvector<int> iv;
        myvector<double> dv;

        EXPECT(true);
    },

    CASE("002: public member types, and sizeof")
    {
        // check public member types for T is char, int, and double
        {
            using vec = myvector<char>;

            EXPECT((std::is_same<char, vec::value_type>::value));
            EXPECT((std::is_same<size_t, vec::size_type>::value));
            EXPECT((std::is_same<ptrdiff_t, vec::difference_type>::value));
            EXPECT((std::is_same<char&, vec::reference>::value));
            EXPECT((std::is_same<const char&, vec::const_reference>::value));
            EXPECT((std::is_same<char*, vec::pointer>::value));
            EXPECT((std::is_same<const char*, vec::const_pointer>::value));
            EXPECT((std::is_same<char*, vec::iterator>::value)); // not mandatory
            EXPECT((std::is_same<const char*, vec::const_iterator>::value)); // not mandatory
            EXPECT((std::is_same<std::reverse_iterator<char*>, vec::reverse_iterator>::value)); // not mandatory
            EXPECT((std::is_same<std::reverse_iterator<const char*>, vec::const_reverse_iterator>::value)); // not mandatory
        }
        {
            using vec = myvector<int>;

            EXPECT((std::is_same<int, vec::value_type>::value));
            EXPECT((std::is_same<size_t, vec::size_type>::value));
            EXPECT((std::is_same<ptrdiff_t, vec::difference_type>::value));
            EXPECT((std::is_same<int&, vec::reference>::value));
            EXPECT((std::is_same<const int&, vec::const_reference>::value));
            EXPECT((std::is_same<int*, vec::pointer>::value));
            EXPECT((std::is_same<const int*, vec::const_pointer>::value));
            EXPECT((std::is_same<int*, vec::iterator>::value)); // not mandatory
            EXPECT((std::is_same<const int*, vec::const_iterator>::value)); // not mandatory
            EXPECT((std::is_same<std::reverse_iterator<int*>, vec::reverse_iterator>::value)); // not mandatory
            EXPECT((std::is_same<std::reverse_iterator<const int*>, vec::const_reverse_iterator>::value)); // not mandatory
        }
        {
            using vec = myvector<double>;

            EXPECT((std::is_same<double, vec::value_type>::value));
            EXPECT((std::is_same<size_t, vec::size_type>::value));
            EXPECT((std::is_same<ptrdiff_t, vec::difference_type>::value));
            EXPECT((std::is_same<double&, vec::reference>::value));
            EXPECT((std::is_same<const double&, vec::const_reference>::value));
            EXPECT((std::is_same<double*, vec::pointer>::value));
            EXPECT((std::is_same<const double*, vec::const_pointer>::value));
            EXPECT((std::is_same<double*, vec::iterator>::value)); // not mandatory
            EXPECT((std::is_same<const double*, vec::const_iterator>::value)); // not mandatory
            EXPECT((std::is_same<std::reverse_iterator<double*>, vec::reverse_iterator>::value)); // not mandatory
            EXPECT((std::is_same<std::reverse_iterator<const double*>, vec::const_reverse_iterator>::value)); // not mandatory
        }

        // check the result of sizeof(). Only minimum check is done
        {
            size_t minimum = sizeof(void*) + sizeof(size_t) + sizeof(size_t);

            EXPECT(minimum <= sizeof(myvector<char>)); // not mandatory
            EXPECT(minimum <= sizeof(myvector<int>)); // not mandatory
            EXPECT(minimum <= sizeof(myvector<double>)); // not mandatory
        }
    },

    CASE("003: default constructor")
    {
        // no actual tests
        EXPECT_NO_THROW(myvector<char> v);
        EXPECT_NO_THROW(myvector<int> v);
        EXPECT_NO_THROW(myvector<double> v);
    },

    CASE("004: size()")
    {
        {
            myvector<char> v;

            EXPECT(0 == v.size());
        }
        {
            myvector<int> v;

            EXPECT(0 == v.size());
        }
        {
            myvector<double> v;

            EXPECT(0 == v.size());
        }
    },

    CASE("005: capacity()")
    {
        {
            myvector<char> v;

            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> v;

            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> v;

            EXPECT(0 == v.capacity());
        }
    },

    CASE("006: begin(), end(), cbegin(), cend()")
    {
        {
            myvector<char> v;

            EXPECT(nullptr == v.begin()); // not mandatory
            EXPECT(nullptr == v.end()); // not mandatory
            EXPECT(nullptr == v.cbegin()); // not mandatory
            EXPECT(nullptr == v.cend()); // not mandatory
            EXPECT(v.begin() == v.end());
            EXPECT(v.cbegin() == v.cend());
            EXPECT(not is_const_lvalue_reference<decltype(*v.begin())>::value);
            EXPECT(not is_const_lvalue_reference<decltype(*v.end())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cbegin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cend())>::value);
        }
        {
            const myvector<char> v;

            EXPECT(nullptr == v.begin()); // not mandatory
            EXPECT(nullptr == v.end()); // not mandatory
            EXPECT(nullptr == v.cbegin()); // not mandatory
            EXPECT(nullptr == v.cend()); // not mandatory
            EXPECT(v.begin() == v.end());
            EXPECT(v.cbegin() == v.cend());
            EXPECT(is_const_lvalue_reference<decltype(*v.begin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.end())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cbegin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cend())>::value);
        }
        {
            myvector<int> v;

            EXPECT(nullptr == v.begin()); // not mandatory
            EXPECT(nullptr == v.end()); // not mandatory
            EXPECT(nullptr == v.cbegin()); // not mandatory
            EXPECT(nullptr == v.cend()); // not mandatory
            EXPECT(v.begin() == v.end());
            EXPECT(v.cbegin() == v.cend());
            EXPECT(not is_const_lvalue_reference<decltype(*v.begin())>::value);
            EXPECT(not is_const_lvalue_reference<decltype(*v.end())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cbegin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cend())>::value);
        }
        {
            const myvector<int> v;

            EXPECT(nullptr == v.begin()); // not mandatory
            EXPECT(nullptr == v.end()); // not mandatory
            EXPECT(nullptr == v.cbegin()); // not mandatory
            EXPECT(nullptr == v.cend()); // not mandatory
            EXPECT(v.begin() == v.end());
            EXPECT(v.cbegin() == v.cend());
            EXPECT(is_const_lvalue_reference<decltype(*v.begin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.end())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cbegin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cend())>::value);
        }
        {
            myvector<double> v;

            EXPECT(nullptr == v.begin()); // not mandatory
            EXPECT(nullptr == v.end()); // not mandatory
            EXPECT(nullptr == v.cbegin()); // not mandatory
            EXPECT(nullptr == v.cend()); // not mandatory
            EXPECT(v.begin() == v.end());
            EXPECT(v.cbegin() == v.cend());
            EXPECT(not is_const_lvalue_reference<decltype(*v.begin())>::value);
            EXPECT(not is_const_lvalue_reference<decltype(*v.end())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cbegin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cend())>::value);
        }
        {
            const myvector<double> v;

            EXPECT(nullptr == v.begin()); // not mandatory
            EXPECT(nullptr == v.end()); // not mandatory
            EXPECT(nullptr == v.cbegin()); // not mandatory
            EXPECT(nullptr == v.cend()); // not mandatory
            EXPECT(v.begin() == v.end());
            EXPECT(v.cbegin() == v.cend());
            EXPECT(is_const_lvalue_reference<decltype(*v.begin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.end())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cbegin())>::value);
            EXPECT(is_const_lvalue_reference<decltype(*v.cend())>::value);
        }
    },

    CASE("007: max_size()")
    {
        {
            myvector<char> v;

            using vec = myvector<char>;
            vec::size_type expected = std::numeric_limits<vec::size_type>::max() / sizeof(vec::value_type);
            EXPECT(expected == v.max_size()); // not mandatory
        }
        {
            myvector<int> v;

            using vec = myvector<int>;
            vec::size_type expected = std::numeric_limits<vec::size_type>::max() / sizeof(vec::value_type);
            EXPECT(expected == v.max_size()); // not mandatory
        }
        {
            myvector<double> v;

            using vec = myvector<double>;
            vec::size_type expected = std::numeric_limits<vec::size_type>::max() / sizeof(vec::value_type);
            EXPECT(expected == v.max_size()); // not mandatory
        }
    },

    CASE("008: constructor with size, destructor")
    {
        {
            myvector<char> v(3);

            EXPECT(char() == *std::next(v.begin(), 0));
            EXPECT(char() == *std::next(v.begin(), 1));
            EXPECT(char() == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.~myvector(); // not mandatory

            EXPECT(v.begin() == v.end()); // not mandatory
            EXPECT(0 == v.size()); // not mandatory
            EXPECT(0 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v(3);

            EXPECT(int() == *std::next(v.begin(), 0));
            EXPECT(int() == *std::next(v.begin(), 1));
            EXPECT(int() == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.~myvector(); // not mandatory

            EXPECT(v.begin() == v.end()); // not mandatory
            EXPECT(0 == v.size()); // not mandatory
            EXPECT(0 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v(3);

            EXPECT(double() == *std::next(v.begin(), 0));
            EXPECT(double() == *std::next(v.begin(), 1));
            EXPECT(double() == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.~myvector(); // not mandatory

            EXPECT(v.begin() == v.end()); // not mandatory
            EXPECT(0 == v.size()); // not mandatory
            EXPECT(0 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v(0);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> v(0);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> v(0);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            auto size = std::numeric_limits<myvector<int>::size_type>::max();
            EXPECT_THROWS_AS((myvector<int>(size)), std::length_error);
        }
        {
            auto size = std::numeric_limits<myvector<double>::size_type>::max();
            EXPECT_THROWS_AS((myvector<double>(size)), std::length_error);
        }
        {
            myvector<noncopyable> v(1);

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());

            v.~myvector(); // not mandatory

            EXPECT(v.begin() == v.end()); // not mandatory
            EXPECT(0 == v.size()); // not mandatory
            EXPECT(0 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());

            v.~myvector(); // not mandatory

            EXPECT(v.begin() == v.end()); // not mandatory
            EXPECT(0 == v.size()); // not mandatory
            EXPECT(0 == v.capacity()); // not mandatory
        }
    },

    CASE("009: constructor with size and initial value")
    {
        {
            myvector<char> v(3, 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT('A' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> v(3, 10);

            EXPECT(10 == *std::next(v.begin(), 0));
            EXPECT(10 == *std::next(v.begin(), 1));
            EXPECT(10 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> v(3, 1.5);

            EXPECT(1.5 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<char> v(0, 'A');

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> v(0, 10);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> v(0, 1.5);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            auto size = std::numeric_limits<myvector<int>::size_type>::max();
            EXPECT_THROWS_AS((myvector<int>(size, 10)), std::length_error);
        }
        {
            auto size = std::numeric_limits<myvector<double>::size_type>::max();
            EXPECT_THROWS_AS((myvector<double>(size, 1.5)), std::length_error);
        }
        {
// noncopyable cannot be applied to initial_value
//            myvector<noncopyable> v(1, noncopyable());
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity());
        }
        {
            myvector<unmovable> v(1, unmovable());

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
        }
    },

    CASE("010: copy constructor")
    {
        {
            myvector<char> src(3, 'A');
            myvector<char> v(src);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT('A' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> src(3, 10);
            myvector<int> v(src);

            EXPECT(10 == *std::next(v.begin(), 0));
            EXPECT(10 == *std::next(v.begin(), 1));
            EXPECT(10 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> src(3, 1.5);
            myvector<double> v(src);

            EXPECT(1.5 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<char> src(0, 'A');
            myvector<char> v(src);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> src(0, 10);
            myvector<int> v(src);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> src(0, 1.5);
            myvector<double> v(src);

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
// noncopyable cannot be applied to copy constructor
//            myvector<noncopyable> src(1, noncopyable());
//            myvector<noncopyable> v(src);
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity());
        }
        {
            myvector<unmovable> src(1, unmovable());
            myvector<unmovable> v(src);

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
        }
    },

    CASE("011: move constructor")
    {
        {
            myvector<char> src(3, 'A');
            myvector<char> v(std::move(src));

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT('A' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<int> src(3, 10);
            myvector<int> v(std::move(src));

            EXPECT(10 == *std::next(v.begin(), 0));
            EXPECT(10 == *std::next(v.begin(), 1));
            EXPECT(10 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<double> src(3, 1.5);
            myvector<double> v(std::move(src));

            EXPECT(1.5 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<char> src(0, 'A');
            myvector<char> v(std::move(src));

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<int> src(0, 10);
            myvector<int> v(std::move(src));

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<double> src(0, 1.5);
            myvector<double> v(std::move(src));

            EXPECT(v.begin() == v.end());
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<noncopyable> src(1);
            myvector<noncopyable> v(std::move(src));

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
        {
            myvector<unmovable> src(1, unmovable());
            myvector<unmovable> v(std::move(src));

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
            EXPECT(src.begin() == src.end());
            EXPECT(0 == src.size());
            EXPECT(0 == src.capacity()); // not mandatory
        }
    },

    CASE("012: constructor with iterator")
    {
        {
            char src[3] = {'A', 'B', 'C'};
            myvector<char> v(std::begin(src), std::end(src));

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            int src[3] = {1, 2, 3};
            myvector<int> v(std::begin(src), std::end(src));

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            double src[3] = {0.5, 1.0, 1.5};
            myvector<double> v(std::begin(src), std::end(src));

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            std::list<char> src = {'A', 'B', 'C'};
            myvector<char> v(src.begin(), src.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            std::list<int> src = {1, 2, 3};
            myvector<int> v(src.begin(), src.end());

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            std::list<double> src = {0.5, 1.0, 1.5};
            myvector<double> v(src.begin(), src.end());

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            std::list<char> src;
            myvector<char> v(src.begin(), src.end());

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            std::list<int> src;
            myvector<int> v(src.begin(), src.end());

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            std::list<double> src;
            myvector<double> v(src.begin(), src.end());

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
// noncopyable cannot make input iterator
//            noncopyable src[1];
//            myvector<noncopyable> v(std::begin(src), std::end(src));
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity());
        }
        {
            unmovable src[1];
            myvector<unmovable> v(std::begin(src), std::end(src));

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
        }
        {
            myvector<size_t> v(2, 3); // constructor with size & initial value should be called

            EXPECT(3 == *std::next(v.begin(), 0));
            EXPECT(3 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity());
        }
    },

    CASE("013: constructor with intializer list")
    {
        {
            myvector<char> v = {'A', 'B', 'C'};

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> v = {1, 2, 3};

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<char> v = {};

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> v = {};

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> v = {};

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
// noncopyable cannot be applied to constructor with initializer_list.
// It is very wired that in some compiler does compile following code,
// as long as noncopyable is trivially copyable.
//            myvector<noncopyable> v = {{}};
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity());
        }
        {
            myvector<unmovable> v = {{}};

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
        }
    },

    CASE("014: reserve()")
    {
        {
            myvector<char> v = {'A', 'B', 'C'};
            v.reserve(2);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(3);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(4);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity());

            v.reserve(50);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(50 == v.capacity());

            v.reserve(0);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(50 == v.capacity());
        }
        {
            myvector<int> v{1, 2, 3};
            v.reserve(2);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(3);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(4);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity());

            v.reserve(50);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(50 == v.capacity());

            v.reserve(0);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(50 == v.capacity());
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};
            v.reserve(2);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(3);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(4);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity());

            v.reserve(50);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(50 == v.capacity());

            v.reserve(0);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(50 == v.capacity());
        }
        {
            myvector<int> v;
            auto size = std::numeric_limits<myvector<int>::size_type>::max();
            EXPECT_THROWS_AS((v.reserve(size)), std::length_error);
        }
        {
            myvector<double> v;
            auto size = std::numeric_limits<myvector<double>::size_type>::max();
            EXPECT_THROWS_AS((v.reserve(size)), std::length_error);
        }
        {
            myvector<noncopyable> v(1);
            v.reserve(1000);

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1000 == v.capacity());
        }
        {
            myvector<unmovable> v(1);
            v.reserve(1000);

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1000 == v.capacity());
        }
    },

    CASE("015: resize()")
    {
        {
            myvector<char> v = {'A', 'B', 'C'};
            v.resize(4);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(char() == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(4);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(char() == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(5);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(char() == *std::next(v.begin(), 3));
            EXPECT(char() == *std::next(v.begin(), 4));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(2);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(0);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {1, 2, 3};
            v.resize(4);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(int() == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(4);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(int() == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(5);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(int() == *std::next(v.begin(), 3));
            EXPECT(int() == *std::next(v.begin(), 4));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(2);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(0);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            v.resize(4);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(double() == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(4);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(double() == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(double() == *std::next(v.begin(), 3));
            EXPECT(double() == *std::next(v.begin(), 4));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(2);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(0);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            auto size = std::numeric_limits<myvector<int>::size_type>::max();
            EXPECT_THROWS_AS((v.resize(size)), std::length_error);
        }
        {
            myvector<double> v;
            auto size = std::numeric_limits<myvector<double>::size_type>::max();
            EXPECT_THROWS_AS((v.resize(size)), std::length_error);
        }
        {
            myvector<noncopyable> v(1);
            v.resize(2);

            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            v.resize(2);

            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'A', 'B', 'C'};
            v.resize(4, 'a');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('a' == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(4, 'b');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('a' == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(5, 'b');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('a' == *std::next(v.begin(), 3));
            EXPECT('b' == *std::next(v.begin(), 4));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(2, 'c');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(0, 'd');

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {1, 2, 3};
            v.resize(4, 8);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(8 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(4, 9);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(8 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(5, 9);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(8 == *std::next(v.begin(), 3));
            EXPECT(9 == *std::next(v.begin(), 4));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(2, 10);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(0, 10);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            v.resize(4, 5.0);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(5.0 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(4, 5.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(5.0 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.resize(5, 5.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(5.0 == *std::next(v.begin(), 3));
            EXPECT(5.5 == *std::next(v.begin(), 4));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(2, 6.0);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            v.resize(0, 6.0);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            auto size = std::numeric_limits<myvector<int>::size_type>::max();
            EXPECT_THROWS_AS((v.resize(size, 1)), std::length_error);
        }
        {
            myvector<double> v;
            auto size = std::numeric_limits<myvector<double>::size_type>::max();
            EXPECT_THROWS_AS((v.resize(size, 0.5)), std::length_error);
        }
        {
// noncopyable cannot be applied to initial_value
//            myvector<noncopyable> v(1);
//            v.resize(2, noncopyable());
//
//            EXPECT(2 == std::distance(v.begin(), v.end()));
//            EXPECT(2 == v.size());
//            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            v.resize(2, unmovable());

            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
    },

    CASE("016: push_back()")
    {
        {
            myvector<char> v;
            char element = 'A';
            v.push_back(element);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            element = 'B';
            v.push_back(element);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            element = 'C';
            v.push_back(element);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            element = 'D';
            v.push_back(element);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('D' == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            int element = 1;
            v.push_back(element);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            element = 2;
            v.push_back(element);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            element = 3;
            v.push_back(element);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            element = 4;
            v.push_back(element);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(4 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            double element = 0.5;
            v.push_back(element);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            element = 1.0;
            v.push_back(element);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            element = 1.5;
            v.push_back(element);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            element = 2.0;
            v.push_back(element);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(2.0 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
// noncopyable cannot be applied to lvalue push_back()
//            myvector<noncopyable> v;
//            noncopyable element;
//            v.push_back(element);
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v;
            unmovable element;
            v.push_back(element);

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            v.push_back('A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            v.push_back('B');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.push_back('C');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.push_back('D');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('D' == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            v.push_back(1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            v.push_back(2);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.push_back(3);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.push_back(4);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(4 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            v.push_back(0.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            v.push_back(1.0);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.push_back(1.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            v.push_back(2.0);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(2.0 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<noncopyable> v;
            v.push_back(noncopyable{});

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
// unmovable cannot be applied to rvalue push_back()
//            myvector<unmovable> v;
//            v.push_back(unmovable{});
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity()); // not mandatory
        }
    },

    CASE("017: operator=()")
    {
        {
            myvector<char> v;
            {
                myvector<char> src{'A', 'B'};
                myvector<char>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT('A' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<char> src{'C', 'B', 'A'};
                myvector<char>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT('C' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT('A' == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<char>& r = (v = v);

                EXPECT(&v == &r);
                EXPECT('C' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT('A' == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<char> src{};
                myvector<char>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<char> src{};
                src.reserve(100);
                myvector<char>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<int> v;
            {
                myvector<int> src{1, 2};
                myvector<int>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(1 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<int> src{3, 2, 1};
                myvector<int>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(3 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(1 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<int>& r = (v = v);

                EXPECT(&v == &r);
                EXPECT(3 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(1 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<int> src{};
                myvector<int>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<int> src{};
                src.reserve(100);
                myvector<int>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<double> v;
            {
                myvector<double> src{0.5, 1.0};
                myvector<double>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<double> src{1.5, 1.0, 0.5};
                myvector<double>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(1.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(0.5 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<double>& r = (v = v);

                EXPECT(&v == &r);
                EXPECT(1.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(0.5 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<double> src{};
                myvector<double>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<double> src{};
                src.reserve(100);
                myvector<double>& r = (v = src);

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
        }
        {
// noncopyable cannot be applied to lvalue operator=()
//            myvector<noncopyable> v;
//            myvector<noncopyable> src(3);
//            myvector<noncopyable>& r = (v = src);
//
//            EXPECT(&v == &r);
//            EXPECT(3 == std::distance(v.begin(), v.end()));
//            EXPECT(3 == v.size());
//            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v;
            myvector<unmovable> src(3);
            myvector<unmovable>& r = (v = src);

            EXPECT(&v == &r);
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            {
                myvector<char> src{'A', 'B'};
                myvector<char>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT('A' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<char> src{'C', 'B', 'A'};
                myvector<char>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT('C' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT('A' == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
// Some compiler does not take care of self move-operator=()
                myvector<char>& r = (v = std::move(v));

                EXPECT(&v == &r);
                EXPECT('C' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT('A' == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<char> src{};
                myvector<char>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(0 == v.capacity()); // not mandatory
            }
            {
                myvector<char> src{};
                src.reserve(100);
                myvector<char>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(100 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<int> v;
            {
                myvector<int> src{1, 2};
                myvector<int>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(1 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<int> src{3, 2, 1};
                myvector<int>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(3 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(1 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
// Some compiler does not take care of self move-operator=()
                myvector<int>& r = (v = std::move(v));

                EXPECT(&v == &r);
                EXPECT(3 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(1 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<int> src{};
                myvector<int>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(0 == v.capacity()); // not mandatory
            }
            {
                myvector<int> src{};
                src.reserve(100);
                myvector<int>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(100 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<double> v;
            {
                myvector<double> src{0.5, 1.0};
                myvector<double>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<double> src{1.5, 1.0, 0.5};
                myvector<double>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(1.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(0.5 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
// Some compiler does not take care of self move-operator=()
                myvector<double>& r = (v = std::move(v));

                EXPECT(&v == &r);
                EXPECT(1.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(0.5 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<double> src{};
                myvector<double>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(0 == v.capacity()); // not mandatory
            }
            {
                myvector<double> src{};
                src.reserve(100);
                myvector<double>& r = (v = std::move(src));

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(100 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<noncopyable> v;
            myvector<noncopyable> src(3);
            myvector<noncopyable>& r = (v = std::move(src));

            EXPECT(&v == &r);
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
// unmovable cannot be applied to rvalue operator=()
//            myvector<unmovable> v;
//            myvector<unmovable> src(3);
//            myvector<unmovable>& r = (v = std::move(src));
//
//            EXPECT(&v == &r);
//            EXPECT(3 == std::distance(v.begin(), v.end()));
//            EXPECT(3 == v.size());
//            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            {
                myvector<char>& r = (v = {'A', 'B'});

                EXPECT(&v == &r);
                EXPECT('A' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<char>& r = (v = {'C', 'B', 'A'});

                EXPECT(&v == &r);
                EXPECT('C' == *std::next(v.begin(), 0));
                EXPECT('B' == *std::next(v.begin(), 1));
                EXPECT('A' == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<char>& r = (v = {});

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<int> v;
            {
                myvector<int>& r = (v = {1, 2});

                EXPECT(&v == &r);
                EXPECT(1 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<int>& r = (v = {3, 2, 1});

                EXPECT(&v == &r);
                EXPECT(3 == *std::next(v.begin(), 0));
                EXPECT(2 == *std::next(v.begin(), 1));
                EXPECT(1 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<int>& r = (v = {});

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
        }
        {
            myvector<double> v;
            {
                myvector<double>& r = (v = {0.5, 1.0});

                EXPECT(&v == &r);
                EXPECT(0.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(2 == std::distance(v.begin(), v.end()));
                EXPECT(2 == v.size());
                EXPECT(2 == v.capacity()); // not mandatory
            }
            {
                myvector<double>& r = (v = {1.5, 1.0, 0.5});

                EXPECT(&v == &r);
                EXPECT(1.5 == *std::next(v.begin(), 0));
                EXPECT(1.0 == *std::next(v.begin(), 1));
                EXPECT(0.5 == *std::next(v.begin(), 2));
                EXPECT(3 == std::distance(v.begin(), v.end()));
                EXPECT(3 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
            {
                myvector<double>& r = (v = {});

                EXPECT(&v == &r);
                EXPECT(0 == std::distance(v.begin(), v.end()));
                EXPECT(0 == v.size());
                EXPECT(3 == v.capacity()); // not mandatory
            }
        }
        {
// noncopyable cannot be applied to initializer_list
//            myvector<noncopyable> v;
//            myvector<noncopyable>& r = (v = {{}});
//
//            EXPECT(&v == &r);
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v;
            myvector<unmovable>& r = (v = {{}});

            EXPECT(&v == &r);
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
    },

    CASE("018: assign()")
    {
        {
            myvector<char> v;
            v.assign(2, 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.assign(3, 'B');

            EXPECT('B' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            v.assign(0, 'C');

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            v.assign(2, 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.assign(3, 2);

            EXPECT(2 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            v.assign(0, 3);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            v.assign(2, 0.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(0.5 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.assign(3, 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.0 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            v.assign(0, 1.5);

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
// noncopyable cannot be applied to initial value of assign()
//            myvector<noncopyable> v;
//            v.assign(1, noncopyable());
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v;
            v.assign(1, unmovable());

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            std::list<char> src = {'A', 'B'};
            v.assign(src.begin(), src.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            src = {'C', 'B', 'A'};
            v.assign(src.begin(), src.end());

            EXPECT('C' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('A' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            src.clear();
            v.assign(src.begin(), src.end());

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            char array[4] = {'A', 'B', 'C', 'D'};
            v.assign(std::begin(array), std::end(array));

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('D' == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            std::list<int> src = {1, 2};
            v.assign(src.begin(), src.end());

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            src = {3, 2, 1};
            v.assign(src.begin(), src.end());

            EXPECT(3 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(1 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            src.clear();
            v.assign(src.begin(), src.end());

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            int array[4] = {1, 2, 3, 4};
            v.assign(std::begin(array), std::end(array));

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(4 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            std::list<double> src = {0.5, 1.0};
            v.assign(src.begin(), src.end());

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            src = {1.5, 1.0, 0.5};
            v.assign(src.begin(), src.end());

            EXPECT(1.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            src.clear();
            v.assign(src.begin(), src.end());

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            double array[4] = {0.5, 1.0, 1.5, 2.0};
            v.assign(std::begin(array), std::end(array));

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(2.0 == *std::next(v.begin(), 3));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
// noncopyable cannot be applied to iterator of assign()
//            myvector<noncopyable> v;
//            noncopyable src[1];
//            v.assign(std::begin(src), std::end(src));
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v;
            unmovable src[1];
            v.assign(std::begin(src), std::end(src));

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            v.assign({'A', 'B'});

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.assign({'C', 'B', 'A'});

            EXPECT('C' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('A' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            v.assign({});

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            v.assign({1, 2});

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.assign({3, 2, 1});

            EXPECT(3 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(1 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            v.assign({});

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            v.assign({0.5, 1.0});

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            v.assign({1.5, 1.0, 0.5});

            EXPECT(1.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            v.assign({});

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
// noncopyable cannot be applied to initializer_list of assign()
//            myvector<noncopyable> v;
//            v.assign({{}});
//
//            EXPECT(1 == std::distance(v.begin(), v.end()));
//            EXPECT(1 == v.size());
//            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v;
            v.assign({{}});

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
    },

    CASE("019: at()")
    {
        {
            myvector<char> v{'A', 'B', 'C'};

            EXPECT('A' == v.at(0));
            EXPECT('B' == v.at(1));
            EXPECT('C' == v.at(2));
            EXPECT_THROWS_AS(v.at(3), std::out_of_range);

            v.at(0) += ('a' - 'A');
            v.at(1) += ('a' - 'A');
            v.at(2) += ('a' - 'A');

            EXPECT('a' == v.at(0));
            EXPECT('b' == v.at(1));
            EXPECT('c' == v.at(2));
            EXPECT_THROWS_AS(v.at(3), std::out_of_range);
        }
        {
            myvector<char> v;

            EXPECT_THROWS_AS(v.at(0), std::out_of_range);
        }
        {
            myvector<char> v{'A'};

            EXPECT(not is_const_lvalue_reference<decltype(v.at(0))>::value);
        }
        {
            const myvector<char> v{'A'};

            EXPECT(is_const_lvalue_reference<decltype(v.at(0))>::value);
        }
        {
            myvector<int> v{1, 2, 3};

            EXPECT(1 == v.at(0));
            EXPECT(2 == v.at(1));
            EXPECT(3 == v.at(2));
            EXPECT_THROWS_AS(v.at(3), std::out_of_range);

            v.at(0) *= 10;
            v.at(1) *= 10;
            v.at(2) *= 10;

            EXPECT(10 == v.at(0));
            EXPECT(20 == v.at(1));
            EXPECT(30 == v.at(2));
            EXPECT_THROWS_AS(v.at(3), std::out_of_range);
        }
        {
            myvector<int> v;

            EXPECT_THROWS_AS(v.at(0), std::out_of_range);
        }
        {
            myvector<int> v{1};

            EXPECT(not is_const_lvalue_reference<decltype(v.at(0))>::value);
        }
        {
            const myvector<int> v{1};

            EXPECT(is_const_lvalue_reference<decltype(v.at(0))>::value);
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};

            EXPECT(0.5 == v.at(0));
            EXPECT(1.0 == v.at(1));
            EXPECT(1.5 == v.at(2));
            EXPECT_THROWS_AS(v.at(3), std::out_of_range);

            v.at(0) *= 4.0;
            v.at(1) *= 4.0;
            v.at(2) *= 4.0;

            EXPECT(2.0 == v.at(0));
            EXPECT(4.0 == v.at(1));
            EXPECT(6.0 == v.at(2));
            EXPECT_THROWS_AS(v.at(3), std::out_of_range);
        }
        {
            myvector<double> v;

            EXPECT_THROWS_AS(v.at(0), std::out_of_range);
        }
        {
            myvector<double> v{1.0};

            EXPECT(not is_const_lvalue_reference<decltype(v.at(0))>::value);
        }
        {
            const myvector<double> v{1.0};

            EXPECT(is_const_lvalue_reference<decltype(v.at(0))>::value);
        }
    },

    CASE("020: operator[]()")
    {
        {
            myvector<char> v{'A', 'B', 'C'};

            EXPECT('A' == v[0]);
            EXPECT('B' == v[1]);
            EXPECT('C' == v[2]);

            v[0] += ('a' - 'A');
            v[1] += ('a' - 'A');
            v[2] += ('a' - 'A');

            EXPECT('a' == v[0]);
            EXPECT('b' == v[1]);
            EXPECT('c' == v[2]);
        }
        {
            myvector<char> v{'A'};

            EXPECT(not is_const_lvalue_reference<decltype(v[0])>::value);
        }
        {
            const myvector<char> v{'A'};

            EXPECT(is_const_lvalue_reference<decltype(v[0])>::value);
        }
        {
            myvector<int> v{1, 2, 3};

            EXPECT(1 == v[0]);
            EXPECT(2 == v[1]);
            EXPECT(3 == v[2]);

            v[0] *= 10;
            v[1] *= 10;
            v[2] *= 10;

            EXPECT(10 == v[0]);
            EXPECT(20 == v[1]);
            EXPECT(30 == v[2]);
        }
        {
            myvector<int> v{1};

            EXPECT(not is_const_lvalue_reference<decltype(v[0])>::value);
        }
        {
            const myvector<int> v{1};

            EXPECT(is_const_lvalue_reference<decltype(v[0])>::value);
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};

            EXPECT(0.5 == v[0]);
            EXPECT(1.0 == v[1]);
            EXPECT(1.5 == v[2]);

            v[0] *= 4.0;
            v[1] *= 4.0;
            v[2] *= 4.0;

            EXPECT(2.0 == v[0]);
            EXPECT(4.0 == v[1]);
            EXPECT(6.0 == v[2]);
        }
        {
            myvector<double> v{1.0};

            EXPECT(not is_const_lvalue_reference<decltype(v[0])>::value);
        }
        {
            const myvector<double> v{1.0};

            EXPECT(is_const_lvalue_reference<decltype(v[0])>::value);
        }
    },

    CASE("021: front(), back()")
    {
        {
            myvector<char> v{'A', 'B', 'C'};

            EXPECT('A' == v.front());
            EXPECT('C' == v.back());

            v.front() += ('a' - 'A');
            v.back() += ('a' - 'A');

            EXPECT('a' == v.front());
            EXPECT('c' == v.back());
        }
        {
            myvector<char> v{'A'};

            EXPECT('A' == v.front());
            EXPECT('A' == v.back());
        }
        {
            myvector<char> v{'A'};

            EXPECT(not is_const_lvalue_reference<decltype(v.front())>::value);
            EXPECT(not is_const_lvalue_reference<decltype(v.back())>::value);
        }
        {
            const myvector<char> v{'A'};

            EXPECT(is_const_lvalue_reference<decltype(v.front())>::value);
            EXPECT(is_const_lvalue_reference<decltype(v.back())>::value);
        }
        {
            myvector<int> v{1, 2, 3};

            EXPECT(1 == v.front());
            EXPECT(3 == v.back());

            v.front() *= 10;
            v.back() *= 10;

            EXPECT(10 == v.front());
            EXPECT(30 == v.back());
        }
        {
            myvector<int> v{1};

            EXPECT(1 == v.front());
            EXPECT(1 == v.back());
        }
        {
            myvector<int> v{1};

            EXPECT(not is_const_lvalue_reference<decltype(v.front())>::value);
            EXPECT(not is_const_lvalue_reference<decltype(v.back())>::value);
        }
        {
            const myvector<int> v{1};

            EXPECT(is_const_lvalue_reference<decltype(v.front())>::value);
            EXPECT(is_const_lvalue_reference<decltype(v.back())>::value);
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};

            EXPECT(0.5 == v.front());
            EXPECT(1.5 == v.back());

            v.front() *= 4.0;
            v.back() *= 4.0;

            EXPECT(2.0 == v.front());
            EXPECT(6.0 == v.back());
        }
        {
            myvector<double> v{1.0};

            EXPECT(1.0 == v.front());
            EXPECT(1.0 == v.back());
        }
        {
            myvector<double> v{1.0};

            EXPECT(not is_const_lvalue_reference<decltype(v.front())>::value);
            EXPECT(not is_const_lvalue_reference<decltype(v.back())>::value);
        }
        {
            const myvector<double> v{1.0};

            EXPECT(is_const_lvalue_reference<decltype(v.front())>::value);
            EXPECT(is_const_lvalue_reference<decltype(v.back())>::value);
        }
    },

    CASE("022: data()")
    {
        {
            myvector<char> v{'A', 'B', 'C'};

            EXPECT((&(*v.begin()) == v.data()));
        }
        {
            myvector<char> v;

            EXPECT((&(*v.begin()) == v.data()));
        }
        {
            myvector<char> v{'A', 'B', 'C'};

            EXPECT(std::is_pointer<decltype(v.data())>::value);
            EXPECT(not is_const_pointer<decltype(v.data())>::value);
        }
        {
            const myvector<char> v{'A', 'B', 'C'};

            EXPECT(std::is_pointer<decltype(v.data())>::value);
            EXPECT(is_const_pointer<decltype(v.data())>::value);
        }
        {
            myvector<int> v{1, 2, 3};

            EXPECT((&(*v.begin()) == v.data()));
        }
        {
            myvector<int> v;

            EXPECT((&(*v.begin()) == v.data()));
        }
        {
            myvector<int> v{1, 2, 3};

            EXPECT(std::is_pointer<decltype(v.data())>::value);
            EXPECT(not is_const_pointer<decltype(v.data())>::value);
        }
        {
            const myvector<int> v{1, 2, 3};

            EXPECT(std::is_pointer<decltype(v.data())>::value);
            EXPECT(is_const_pointer<decltype(v.data())>::value);
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};

            EXPECT((&(*v.begin()) == v.data()));
        }
        {
            myvector<double> v;

            EXPECT((&(*v.begin()) == v.data()));
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};

            EXPECT(std::is_pointer<decltype(v.data())>::value);
            EXPECT(not is_const_pointer<decltype(v.data())>::value);
        }
        {
            const myvector<double> v{0.5, 1.0, 1.5};

            EXPECT(std::is_pointer<decltype(v.data())>::value);
            EXPECT(is_const_pointer<decltype(v.data())>::value);
        }
    },

    CASE("023: empty()")
    {
        {
            myvector<char> v{'A', 'B', 'C'};

            EXPECT(false == v.empty());

            v.resize(0);

            EXPECT(true == v.empty());

            v.resize(2);

            EXPECT(false == v.empty());
        }
        {
            myvector<char> v;

            EXPECT(true == v.empty());
        }
        {
            myvector<int> v{1, 2, 3};

            EXPECT(false == v.empty());

            v.resize(0);

            EXPECT(true == v.empty());

            v.resize(2);

            EXPECT(false == v.empty());
        }
        {
            myvector<int> v;

            EXPECT(true == v.empty());
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};

            EXPECT(false == v.empty());

            v.resize(0);

            EXPECT(true == v.empty());

            v.resize(2);

            EXPECT(false == v.empty());
        }
        {
            myvector<double> v;

            EXPECT(true == v.empty());
        }
    },

    CASE("024: shrink_to_fit()")
    {
        {
            myvector<char> v{'A', 'B', 'C'};
            v.shrink_to_fit();

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(10);
            v.shrink_to_fit();

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.resize(2);
            v.shrink_to_fit();

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity());

            v.resize(0);
            v.shrink_to_fit();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> v{1, 2, 3};
            v.shrink_to_fit();

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(10);
            v.shrink_to_fit();

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.resize(2);
            v.shrink_to_fit();

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity());

            v.resize(0);
            v.shrink_to_fit();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> v{0.5, 1.0, 1.5};
            v.shrink_to_fit();

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.reserve(10);
            v.shrink_to_fit();

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity());

            v.resize(2);
            v.shrink_to_fit();

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity());

            v.resize(0);
            v.shrink_to_fit();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<noncopyable> v(3);
            v.push_back(noncopyable{});
            v.shrink_to_fit();

            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity());

            v.resize(1);
            v.shrink_to_fit();

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
        }
        {
            myvector<unmovable> v(3);
            unmovable element;
            v.push_back(element);
            v.shrink_to_fit();

            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity());

            v.resize(1);
            v.shrink_to_fit();

            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity());
        }
    },

    CASE("025: clear()")
    {
        {
            myvector<char> v = {'A', 'B', 'C'};
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<char> v;
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<int> v = {1, 2, 3};
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> v;
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> v;
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(0 == v.capacity());
        }
        {
            myvector<noncopyable> v(3);
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<unmovable> v(3);
            v.clear();

            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
    },

    CASE("026: insert()")
    {
        {
            myvector<char> v = {'B'};
            char item = 'A';
            auto i = v.insert(v.begin(), item);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            item = 'C';
            i = v.insert(v.end(), item);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            item = 'a';
            i = v.insert(v.begin() + 1, item);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('a' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT('C' == *std::next(v.begin(), 3));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            char item = 'A';
            auto i = v.insert(v.begin(), item);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            char item = 'A';
            auto i = v.insert(v.end(), item);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {2};
            int item = 1;
            auto i = v.insert(v.begin(), item);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            item = 3;
            i = v.insert(v.end(), item);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            item = 0;
            i = v.insert(v.begin() + 1, item);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(0 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(3 == *std::next(v.begin(), 3));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            int item = 1;
            auto i = v.insert(v.begin(), item);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            int item = 1;
            auto i = v.insert(v.end(), item);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {1.5};
            double item = 1.0;
            auto i = v.insert(v.begin(), item);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            item = 2.0;
            i = v.insert(v.end(), item);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(2.0 == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            item = 0.0;
            i = v.insert(v.begin() + 1, item);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(0.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(2.0 == *std::next(v.begin(), 3));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            double item = 1.0;
            auto i = v.insert(v.begin(), item);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            double item = 1.0;
            auto i = v.insert(v.end(), item);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            unmovable element;
            auto i = v.insert(v.begin(), element);

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'B'};
            auto i = v.insert(v.begin(), 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            i = v.insert(v.end(), 'C');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 1, 'a');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('a' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT('C' == *std::next(v.begin(), 3));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            auto i = v.insert(v.begin(), 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v;
            auto i = v.insert(v.end(), 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {2};
            auto i = v.insert(v.begin(), 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            i = v.insert(v.end(), 3);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 1, 0);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(0 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(3 == *std::next(v.begin(), 3));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            auto i = v.insert(v.begin(), 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            auto i = v.insert(v.end(), 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {1.5};
            auto i = v.insert(v.begin(), 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            i = v.insert(v.end(), 2.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(2.0 == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 1, 0.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(0.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(2.0 == *std::next(v.begin(), 3));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            auto i = v.insert(v.begin(), 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            auto i = v.insert(v.end(), 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory
        }
        {
            myvector<noncopyable> v(1);
            auto i = v.insert(v.begin(), noncopyable());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'B'};
            auto i = v.insert(v.begin(), 2, 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            i = v.insert(v.end(), 2, 'C');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT('C' == *std::next(v.begin(), 3));
            EXPECT('C' == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 2, 1, 'a');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT('a' == *std::next(v.begin(), 2));
            EXPECT('B' == *std::next(v.begin(), 3));
            EXPECT('C' == *std::next(v.begin(), 4));
            EXPECT('C' == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {};
            auto i = v.insert(v.begin(), 2, 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {};
            auto i = v.insert(v.end(), 2, 'A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('A' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'A', 'B', 'C'};
            auto i = v.insert(std::next(v.begin(), 1), 0, 'a');

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {2};
            auto i = v.insert(v.begin(), 2, 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            i = v.insert(v.end(), 2, 3);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(3 == *std::next(v.begin(), 3));
            EXPECT(3 == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 2, 1, 0);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(0 == *std::next(v.begin(), 2));
            EXPECT(2 == *std::next(v.begin(), 3));
            EXPECT(3 == *std::next(v.begin(), 4));
            EXPECT(3 == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            auto i = v.insert(v.begin(), 2, 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            auto i = v.insert(v.end(), 2, 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {1, 2, 3};
            auto i = v.insert(std::next(v.begin(), 1), 0, 4);

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {1.5};
            auto i = v.insert(v.begin(), 2, 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            i = v.insert(v.end(), 2, 2.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT(2.0 == *std::next(v.begin(), 3));
            EXPECT(2.0 == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 2, 1, 0.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.0 == *std::next(v.begin(), 2));
            EXPECT(1.5 == *std::next(v.begin(), 3));
            EXPECT(2.0 == *std::next(v.begin(), 4));
            EXPECT(2.0 == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {};
            auto i = v.insert(v.begin(), 2, 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {};
            auto i = v.insert(v.end(), 2, 1.0);

            EXPECT(1.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {1.0, 1.5, 2.0};
            auto i = v.insert(std::next(v.begin(), 1), 0, 0.0);

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            unmovable element;
            auto i = v.insert(v.begin(), 2, element);

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'B'};
            std::list<char> src_list = {'A', 'C'};
            auto i = v.insert(v.begin(), src_list.begin(), src_list.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('C' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            char src_array[2] = {'C', 'A'};
            i = v.insert(v.end(), std::begin(src_array), std::end(src_array));

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('C' == *std::next(v.begin(), 1));
            EXPECT('B' == *std::next(v.begin(), 2));
            EXPECT('C' == *std::next(v.begin(), 3));
            EXPECT('A' == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            src_list = {'a'};
            i = v.insert(v.begin() + 2, src_list.begin(), src_list.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('C' == *std::next(v.begin(), 1));
            EXPECT('a' == *std::next(v.begin(), 2));
            EXPECT('B' == *std::next(v.begin(), 3));
            EXPECT('C' == *std::next(v.begin(), 4));
            EXPECT('A' == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {};
            std::list<char> src_list = {'A', 'B'};
            auto i = v.insert(v.begin(), src_list.begin(), src_list.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {};
            std::list<char> src_list = {'A', 'B'};
            auto i = v.insert(v.end(), src_list.begin(), src_list.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'A', 'B', 'C'};
            std::list<char> src_list = {};
            auto i = v.insert(std::next(v.begin(), 1), src_list.begin(), src_list.end());

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {1};
            std::list<int> src_list = {0, 2};
            auto i = v.insert(v.begin(), src_list.begin(), src_list.end());

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(1 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            int src_array[2] = {2, 0};
            i = v.insert(v.end(), std::begin(src_array), std::end(src_array));

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(1 == *std::next(v.begin(), 2));
            EXPECT(2 == *std::next(v.begin(), 3));
            EXPECT(0 == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            src_list = {-1};
            i = v.insert(v.begin() + 2, src_list.begin(), src_list.end());

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(-1 == *std::next(v.begin(), 2));
            EXPECT(1 == *std::next(v.begin(), 3));
            EXPECT(2 == *std::next(v.begin(), 4));
            EXPECT(0 == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            std::list<int> src_list = {1, 2};
            auto i = v.insert(v.begin(), src_list.begin(), src_list.end());

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            std::list<int> src_list = {1, 2};
            auto i = v.insert(v.end(), src_list.begin(), src_list.end());

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {1, 2, 3};
            std::list<int> src_list = {};
            auto i = v.insert(std::next(v.begin(), 1), src_list.begin(), src_list.end());

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {0.5};
            std::list<double> src_list = {0.0, 1.0};
            auto i = v.insert(v.begin(), src_list.begin(), src_list.end());

            EXPECT(0.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.5 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            double src_array[2] = {1.0, 0.0};
            i = v.insert(v.end(), std::begin(src_array), std::end(src_array));

            EXPECT(0.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.5 == *std::next(v.begin(), 2));
            EXPECT(1.0 == *std::next(v.begin(), 3));
            EXPECT(0.0 == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            src_list = {-1.0};
            i = v.insert(v.begin() + 2, src_list.begin(), src_list.end());

            EXPECT(0.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(-1.0 == *std::next(v.begin(), 2));
            EXPECT(0.5 == *std::next(v.begin(), 3));
            EXPECT(1.0 == *std::next(v.begin(), 4));
            EXPECT(0.0 == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {};
            std::list<double> src_list = {0.5, 1.0};
            auto i = v.insert(v.begin(), src_list.begin(), src_list.end());

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {};
            std::list<double> src_list = {0.5, 1.0};
            auto i = v.insert(v.end(), src_list.begin(), src_list.end());

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            std::list<double> src_list = {};
            auto i = v.insert(std::next(v.begin(), 1), src_list.begin(), src_list.end());

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(3);
            std::list<unmovable> element(2);
            auto i  = v.insert(v.begin(), element.begin(), element.end());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory
        }
        {
            myvector<char> v = {'C'};
            auto i = v.insert(v.begin(), {'A', 'B'});

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            i = v.insert(v.end(), {'B', 'A'});

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT('B' == *std::next(v.begin(), 3));
            EXPECT('A' == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 2, {'a'});

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('a' == *std::next(v.begin(), 2));
            EXPECT('C' == *std::next(v.begin(), 3));
            EXPECT('B' == *std::next(v.begin(), 4));
            EXPECT('A' == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            auto i = v.insert(v.begin(), {'A', 'B'});

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            auto i = v.insert(v.end(), {'A', 'B'});

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {'A', 'B', 'C'};
            auto i = v.insert(std::next(v.begin(), 1), {});

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {2};
            auto i = v.insert(v.begin(), {0, 1});

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            i = v.insert(v.end(), {1, 0});

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(1 == *std::next(v.begin(), 3));
            EXPECT(0 == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 2, {-1});

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(-1 == *std::next(v.begin(), 2));
            EXPECT(2 == *std::next(v.begin(), 3));
            EXPECT(1 == *std::next(v.begin(), 4));
            EXPECT(0 == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            auto i = v.insert(v.begin(), {1, 2});

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {};
            auto i = v.insert(v.end(), {1, 2});

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v = {1, 2, 3};
            auto i = v.insert(std::next(v.begin(), 1), {});

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {0.5};
            auto i = v.insert(v.begin(), {0.0, 1.0});

            EXPECT(0.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.5 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory

            i = v.insert(v.end(), {1.0, 0.0});

            EXPECT(0.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0.5 == *std::next(v.begin(), 2));
            EXPECT(1.0 == *std::next(v.begin(), 3));
            EXPECT(0.0 == *std::next(v.begin(), 4));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(5 == v.capacity()); // not mandatory

            i = v.insert(v.begin() + 2, {-1.0});

            EXPECT(0.0 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(-1.0 == *std::next(v.begin(), 2));
            EXPECT(0.5 == *std::next(v.begin(), 3));
            EXPECT(1.0 == *std::next(v.begin(), 4));
            EXPECT(0.0 == *std::next(v.begin(), 5));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(6 == std::distance(v.begin(), v.end()));
            EXPECT(6 == v.size());
            EXPECT(6 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {};
            auto i = v.insert(v.begin(), {0.5, 1.0});

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {};
            auto i = v.insert(v.end(), {0.5, 1.0});

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            auto i = v.insert(std::next(v.begin(), 1), {});

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            auto i = v.insert(v.begin(), {{}, {}});

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(3 == v.capacity()); // not mandatory
        }
    },

    CASE("027: emplace()")
    {
        {
            myvector<int> v{1, 2};
            auto i = v.emplace(v.begin(), 0);

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            i = v.emplace(v.end(), 3);

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(2 == *std::next(v.begin(), 2));
            EXPECT(3 == *std::next(v.begin(), 3));
            EXPECT(3 == std::distance(v.begin(), i));
            EXPECT(4 == std::distance(v.begin(), v.end()));
            EXPECT(4 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory

            i = v.emplace(v.begin() + 2, 10);

            EXPECT(0 == *std::next(v.begin(), 0));
            EXPECT(1 == *std::next(v.begin(), 1));
            EXPECT(10 == *std::next(v.begin(), 2));
            EXPECT(2 == *std::next(v.begin(), 3));
            EXPECT(3 == *std::next(v.begin(), 4));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(5 == std::distance(v.begin(), v.end()));
            EXPECT(5 == v.size());
            EXPECT(8 == v.capacity()); // not mandatory
        }
        {
            using int_pair = std::pair<int, int>;

            myvector<int_pair> v{{1, 10}, {2, 20}};
            auto i = v.emplace(v.end(), 3, 30);

            EXPECT(int_pair(1, 10) == *std::next(v.begin(), 0));
            EXPECT(int_pair(2, 20) == *std::next(v.begin(), 1));
            EXPECT(int_pair(3, 30) == *std::next(v.begin(), 2));
            EXPECT(2 == std::distance(v.begin(), i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            auto i = v.emplace(v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<std::list<int>> v(1);
            auto i = v.emplace(v.begin(), 2, 5);

            EXPECT(5 == v.front().front());
            EXPECT(5 == v.front().back());
            EXPECT(2 == v.front().size());
            EXPECT(0 == v.back().size());
            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
    },

    CASE("028: erase()")
    {
        {
            myvector<char> v = {'A', 'B', 'C'};
            auto i = v.erase(v.begin() + 1);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('C' == *std::next(v.begin(), 1));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin() + 1);

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> v = {1, 2, 3};
            auto i = v.erase(v.begin() + 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(3 == *std::next(v.begin(), 1));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin() + 1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            auto i = v.erase(v.begin() + 1);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.5 == *std::next(v.begin(), 1));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin() + 1);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<noncopyable> v(3);
            auto i = v.erase(v.begin() + 1);

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<unmovable> v(3);
            auto i = v.erase(v.begin() + 1);

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<char> v = {'A', 'B', 'C'};
            auto i = v.erase(v.begin() + 1, v.end());

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin(), v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> v = {1, 2, 3};
            auto i = v.erase(v.begin() + 1, v.end());

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin(), v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            auto i = v.erase(v.begin() + 1, v.end());

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin(), v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<char> v = {'A', 'B', 'C'};
            auto i = v.erase(v.begin(), v.end());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin(), v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<int> v = {1, 2, 3};
            auto i = v.erase(v.begin(), v.end());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin(), v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<double> v = {0.5, 1.0, 1.5};
            auto i = v.erase(v.begin(), v.end());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());

            i = v.erase(v.begin(), v.begin());

            EXPECT(0 == std::distance(v.begin(), i));
            EXPECT(0 == std::distance(v.begin(), v.end()));
            EXPECT(0 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<noncopyable> v(3);
            auto i = v.erase(v.begin() + 1, v.end());

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());
        }
        {
            myvector<unmovable> v(3);
            auto i = v.erase(v.begin() + 1, v.end());

            EXPECT(1 == std::distance(v.begin(), i));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(3 == v.capacity());
        }
    },

    CASE("029: emplace_back()")
    {
        {
            myvector<char> v;
            auto& i1 = v.emplace_back('A');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT((&(v[0]) == &i1));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            auto& i2 = v.emplace_back('B');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT((&(v[1]) == &i2));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            auto& i3 = v.emplace_back('C');

            EXPECT('A' == *std::next(v.begin(), 0));
            EXPECT('B' == *std::next(v.begin(), 1));
            EXPECT('C' == *std::next(v.begin(), 2));
            EXPECT((&(v[2]) == &i3));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<int> v;
            auto& i1 = v.emplace_back(1);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT((&(v[0]) == &i1));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            auto& i2 = v.emplace_back(2);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT((&(v[1]) == &i2));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            auto& i3 = v.emplace_back(3);

            EXPECT(1 == *std::next(v.begin(), 0));
            EXPECT(2 == *std::next(v.begin(), 1));
            EXPECT(3 == *std::next(v.begin(), 2));
            EXPECT((&(v[2]) == &i3));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<double> v;
            auto& i1 = v.emplace_back(0.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT((&(v[0]) == &i1));
            EXPECT(1 == std::distance(v.begin(), v.end()));
            EXPECT(1 == v.size());
            EXPECT(1 == v.capacity()); // not mandatory

            auto& i2 = v.emplace_back(1.0);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT((&(v[1]) == &i2));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory

            auto& i3 = v.emplace_back(1.5);

            EXPECT(0.5 == *std::next(v.begin(), 0));
            EXPECT(1.0 == *std::next(v.begin(), 1));
            EXPECT(1.5 == *std::next(v.begin(), 2));
            EXPECT((&(v[2]) == &i3));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            using int_pair = std::pair<int, int>;

            myvector<int_pair> v{{1, 10}, {2, 20}};
            auto& i = v.emplace_back(3, 30);

            EXPECT(int_pair(1, 10) == *std::next(v.begin(), 0));
            EXPECT(int_pair(2, 20) == *std::next(v.begin(), 1));
            EXPECT(int_pair(3, 30) == *std::next(v.begin(), 2));
            EXPECT((&(v[2]) == &i));
            EXPECT(3 == std::distance(v.begin(), v.end()));
            EXPECT(3 == v.size());
            EXPECT(4 == v.capacity()); // not mandatory
        }
        {
            myvector<unmovable> v(1);
            auto& i = v.emplace_back();

            EXPECT((&(v[1]) == &i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
        {
            myvector<std::list<int>> v(1);
            auto& i = v.emplace_back(2, 5);

            EXPECT(0 == v.front().size());
            EXPECT(5 == v.back().front());
            EXPECT(5 == v.back().back());
            EXPECT(2 == v.back().size());
            EXPECT((&(v[1]) == &i));
            EXPECT(2 == std::distance(v.begin(), v.end()));
            EXPECT(2 == v.size());
            EXPECT(2 == v.capacity()); // not mandatory
        }
    },

};

int main(int argc, char* argv[])
{
    return lest::run(specification, argc, argv);
}
/////////////////////////////////////////////////////////////////////////////
