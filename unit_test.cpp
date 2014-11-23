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
        stream << std::endl << "\033[1m=== Plan " << plan_name << " ===\033[0m" << std::endl;
    }

    ++next_test_index;
    bool result = checkTest(test);

    std::string result_name = (result ? "pass" : "fail");
    std::string formatting = (result ? "32;1" : "31;1");

    stream << "\033[" << formatting << "m" << std::setw(3) << next_test_index << ". " << result_name << ": " << test_name << "\033[0m" << std::endl;
}


template <typename TestType>
UnitTester<TestType>::~UnitTester() {}
