#ifndef UNIT_TEST_H_INCLUDED
#define UNIT_TEST_H_INCLUDED

#include <iostream>
#include <string>

#include "coordinate.h"


template <typename TestType>
class UnitTester
{
    const std::string plan_name;
    std::ostream &stream;
    int next_test_index;
public:
    UnitTester(const std::string &plan_name_tmp, std::ostream &stream_tmp = std::cout);

    void runTest(const std::string &test_name, const TestType &test);

    virtual bool checkTest(const TestType &test) const = 0;

    virtual ~UnitTester();
};


#endif // UNIT_TEST_H_INCLUDED
