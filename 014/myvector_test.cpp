/**
 * @file    myvector_test.cpp
 * @brief   test of myvector using lest. 
 * @author  toda
 * @date    2019-05-19
 * @version 0.1.0
 * @remark  the target is C++11 or more.
 */

#include <list>
#include <type_traits>
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

};

int main(int argc, char* argv[])
{
    return lest::run(specification, argc, argv);
}
/////////////////////////////////////////////////////////////////////////////
