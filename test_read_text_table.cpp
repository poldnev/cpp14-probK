#include <vector>
#include <sstream>

#include "unit_test.h"
#include "unit_test.cpp"
#include "coordinate.h"
#include "sparse_table.h"
#include "sparse_table.cpp"
#include "expression_table.h"


using TextTableEntry = SparseTable<std::string>::Entry;


struct ReadTextTableTest
{
    std::string raw_input;
    int height;
    int width;
    std::vector<TextTableEntry> entries;
    bool is_error;

    // success test
    ReadTextTableTest(const std::string &raw_input_tmp, int height_tmp, int width_tmp, const std::vector<TextTableEntry> &entries_tmp)
        : raw_input(raw_input_tmp), height(height_tmp), width(width_tmp), entries(entries_tmp), is_error(false) {}

    // failure test
    ReadTextTableTest(const std::string &raw_input_tmp)
        : raw_input(raw_input_tmp), height(), width(), entries(), is_error(true) {}
};


class UTReadTextTable : public UnitTester<ReadTextTableTest>
{
public:
    UTReadTextTable(const std::string &plan_name) : UnitTester(plan_name) {}

    bool checkTest(const ReadTextTableTest &test) const {

        try {
            auto expected = makeSparseTable(test.height, test.width, test.entries.begin(), test.entries.end());
            std::stringstream in_stream(test.raw_input);
            auto got = readTextTable(in_stream);
            return expected == got;
        } catch (const std::exception&) {
            return test.is_error;
        }

    }
};


int main()
{
    UTReadTextTable tester("readTextTable");

    tester.runTest("1x1, empty",
        {"1\t1", 1, 1, {}}
    );
    tester.runTest("1x1, empty",
        {"1\t1\n", 1, 1, {}}
    );

    tester.runTest("0x0, empty",
        {"100\t0\n", 0, 0, {}}
    );

    tester.runTest("1x1, full",
        {"1\t1\nHi!", 1, 1, {
            {{0, 0}, "Hi!"},
        }}
    );

    tester.runTest("2x3, almost full",
        {"2\t3\nOne\tTwo\tThree\n\t\tSix\n", 2, 3, {
            {{0, 0}, "One"},
            {{0, 1}, "Two"},
            {{0, 2}, "Three"},
            {{1, 2}, "Six"},
        }}
    );
    tester.runTest("2x3, almost full, with CRLF",
        {"2\t3\r\nOne\tTwo\tThree\r\n\t\tSix\r\n", 2, 3, {
            {{0, 0}, "One"},
            {{0, 1}, "Two"},
            {{0, 2}, "Three"},
            {{1, 2}, "Six"},
        }}
    );
    tester.runTest("2x3, full",
        {"2\t3\nOne\tTwo\tThree\nFour\t \tSix\n", 2, 3, {
            {{0, 0}, "One"},
            {{0, 1}, "Two"},
            {{0, 2}, "Three"},
            {{1, 2}, "Six"},
            {{1, 0}, "Four"},
            {{1, 1}, " "},
        }}
    );

    tester.runTest("8x5, truncated",
        {"8\t5\nOne\tTwo\tThree\nFour\t \tSix\n", 8, 5, {
            {{0, 0}, "One"},
            {{0, 1}, "Two"},
            {{0, 2}, "Three"},
            {{1, 2}, "Six"},
            {{1, 0}, "Four"},
            {{1, 1}, " "},
        }}
    );

    tester.runTest("Negative height",
        {"-5\t8\n"}
    );
    tester.runTest("Negative width",
        {"3\t-1\n"}
    );
    tester.runTest("No width",
        {"3\t\n"}
    );
    tester.runTest("No width",
        {"9"}
    );
    tester.runTest("NaN width",
        {"9\tnan"}
    );
    tester.runTest("NaN sizes",
        {"abc\tcds"}
    );
    tester.runTest("Trash",
        {"d9fsdu9f"}
    );

    return 0;
}
