#include <iostream>
#include <string>
#include <iomanip>

#include "unit_test.h"


template <typename TestType>
UnitTester<TestType>::UnitTester(const std::string &plan_name_tmp, std::ostream &stream_tmp)
    : plan_name(plan_name_tmp), stream(stream_tmp), next_test_index(0) {}


template <typename TestType>
void UnitTester<TestType>::runTest(const std::string &test_name, const TestType &test) {
    if (next_test_index == 0) {
        stream << std::endl << "=== Plan " << plan_name << " ===" << std::endl;
    }

    ++next_test_index;
    bool result = checkTest(test);

    stream << std::setw(3) << next_test_index << ". " << (result ? "pass" : "fail") << ": " << test_name << std::endl;
}


template <typename TestType>
UnitTester<TestType>::~UnitTester() {}
