/**
 * @file    myvector_test.cpp
 * @brief   test of myvector using lest. 
 * @author  toda
 * @date    2019-05-06
 * @version 0.1.0
 * @remark  the target is C++11 or more.
 */

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

};

int main(int argc, char* argv[])
{
    return lest::run(specification, argc, argv);
}
/////////////////////////////////////////////////////////////////////////////
