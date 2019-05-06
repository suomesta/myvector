/**
 * @file    myvector_test.cpp
 * @brief   test of myvector using lest. 
 * @author  toda
 * @date    2019-05-06
 * @version 0.1.0
 * @remark  the target is C++11 or more.
 */

#include <type_traits>
#include "../include/lest.hpp"
#include "myvector.hpp"

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

};

int main(int argc, char* argv[])
{
    return lest::run(specification, argc, argv);
}
/////////////////////////////////////////////////////////////////////////////
