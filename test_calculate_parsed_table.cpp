#include <vector>
#include <sstream>
#include <memory>

#include "unit_test.h"
#include "unit_test.cpp"
#include "coordinate.h"
#include "expression.h"
#include "sparse_table.h"
#include "sparse_table.cpp"
#include "expression_table.h"


using ExpressionTableEntry = SparseTable<Expression>::Entry;


struct CalculateParsedTableTest
{
    int height;
    int width;
    std::vector<ExpressionTableEntry> input_entries;
    std::vector<ExpressionTableEntry> calculated_entries;

    CalculateParsedTableTest(
        int height_tmp, int width_tmp,
        const std::vector<ExpressionTableEntry> &input_entries_tmp,
        const std::vector<ExpressionTableEntry> &calculated_entries_tmp
    ) : height(height_tmp), width(width_tmp), input_entries(input_entries_tmp), calculated_entries(calculated_entries_tmp) {}
};


class UTCalculateParsedTable : public UnitTester<CalculateParsedTableTest>
{
public:
    UTCalculateParsedTable(const std::string &plan_name) : UnitTester(plan_name) {}

    bool checkTest(const CalculateParsedTableTest &test) const {

        auto input_table = makeSparseTable(test.height, test.width, test.input_entries.begin(), test.input_entries.end());
        auto expected = makeSparseTable(test.height, test.width, test.calculated_entries.begin(), test.calculated_entries.end());
        calculateParsedTable(input_table);

        return input_table == expected;

    }
};


int main()
{
    UTCalculateParsedTable tester("calculateParsedTable");

    using LexemVector = std::vector<std::shared_ptr<LexemBase>>;

    tester.runTest("Simple 1x1 arithmetic",
        {
            1, 1,
            {
                {{0, 0}, parseExpression("=2*2")},
            }, {
                {{0, 0}, parseExpression("4")},
            }
        }
    );

    tester.runTest("2x3 with expressions and string",
        {
            2, 3,
            {
                {{0, 0}, parseExpression("=8/3+1")},
                {{0, 1}, parseExpression("=A1+100")},
                {{1, 0}, parseExpression("=B1*9")},
                {{1, 1}, parseExpression("=B1*A2")},
                {{1, 2}, parseExpression("'String")},
            }, {
                {{0, 0}, parseExpression("3")},
                {{0, 1}, parseExpression("103")},
                {{1, 0}, parseExpression("927")},
                {{1, 1}, parseExpression("95481")},
                {{1, 2}, parseExpression("'String")},
            }
        }
    );

    tester.runTest("1x1 with negative result",
        {
            1, 1,
            {
                {{0, 0}, parseExpression("=5-9")},
            }, {
                {{0, 0}, {ExpressionType::ARITHMETIC, LexemVector{std::make_shared<LexemNumber>(-4)}}},
            }
        }
    );

    tester.runTest("3x4 sample from presentation",
        {
            3, 4,
            {
                {{0, 0}, parseExpression("12")},
                {{0, 1}, parseExpression("=C2")},
                {{0, 2}, parseExpression("3")},
                {{0, 3}, parseExpression("'Sample")},
                {{1, 0}, parseExpression("=A1+B1*C1/5")},
                {{1, 1}, parseExpression("=A2*B1")},
                {{1, 2}, parseExpression("=B3-C3")},
                {{1, 3}, parseExpression("'Spread")},
                {{2, 0}, parseExpression("'Test")},
                {{2, 1}, parseExpression("=4-3")},
                {{2, 2}, parseExpression("5")},
                {{2, 3}, parseExpression("'Sheet")},
            }, {
                {{0, 0}, parseExpression("12")},
                {{0, 1}, {ExpressionType::ARITHMETIC, LexemVector{std::make_shared<LexemNumber>(-4)}}},
                {{0, 2}, parseExpression("3")},
                {{0, 3}, parseExpression("'Sample")},
                {{1, 0}, parseExpression("4")},
                {{1, 1}, {ExpressionType::ARITHMETIC, LexemVector{std::make_shared<LexemNumber>(-16)}}},
                {{1, 2}, {ExpressionType::ARITHMETIC, LexemVector{std::make_shared<LexemNumber>(-4)}}},
                {{1, 3}, parseExpression("'Spread")},
                {{2, 0}, parseExpression("'Test")},
                {{2, 1}, parseExpression("1")},
                {{2, 2}, parseExpression("5")},
                {{2, 3}, parseExpression("'Sheet")},
            }
        }
    );

    tester.runTest("1x1 with negative result",
        {
            1, 1,
            {
                {{0, 0}, parseExpression("=5-9")},
            }, {
                {{0, 0}, {ExpressionType::ARITHMETIC, LexemVector{std::make_shared<LexemNumber>(-4)}}},
            }
        }
    );

    tester.runTest("4x4 with expressions and empty strings",
        {
            4, 4,
            {
                {{0, 0}, parseExpression("=1+9")},
                {{0, 1}, parseExpression("=A2")},
                {{0, 2}, parseExpression("'")},
                {{0, 3}, parseExpression("=0")},
                {{1, 0}, parseExpression("=8*1")},
                {{1, 3}, parseExpression("=B3+B1+B3")},
                {{2, 0}, parseExpression("0")},
                {{2, 1}, parseExpression("=A2+A1")},
                {{2, 2}, parseExpression("=1+2*3/4-5+100")},
                {{2, 3}, parseExpression("'")},
            }, {
                {{0, 0}, parseExpression("10")},
                {{0, 1}, parseExpression("8")},
                {{0, 2}, parseExpression("'")},
                {{0, 3}, parseExpression("0")},
                {{1, 0}, parseExpression("8")},
                {{1, 3}, parseExpression("44")},
                {{2, 0}, parseExpression("0")},
                {{2, 1}, parseExpression("18")},
                {{2, 2}, parseExpression("97")},
                {{2, 3}, parseExpression("'")},
            }
        }
    );

    return 0;
}
