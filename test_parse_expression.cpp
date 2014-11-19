#include <memory>

#include "unit_test.h"
#include "unit_test.cpp"
#include "coordinate.h"
#include "expression.h"


struct ParseExpressionTest
{
    std::string raw_expression;
    Expression expression;
    bool is_error;

    // success test
    ParseExpressionTest(const std::string &raw_expression_tmp, const Expression &expression_tmp)
        : raw_expression(raw_expression_tmp), expression(expression_tmp), is_error(false) {}

    // failure test
    ParseExpressionTest(const std::string &raw_expression_tmp)
        : raw_expression(raw_expression_tmp), expression(), is_error(true) {}
};


class UTParseExpression : public UnitTester<ParseExpressionTest>
{
public:
    UTParseExpression(const std::string &plan_name) : UnitTester(plan_name) {}

    bool checkTest(const ParseExpressionTest &test) const {
        if (test.is_error) {
            return parseExpression(test.raw_expression).getType() == ExpressionType::ERROR;
        } else {
            return test.expression == parseExpression(test.raw_expression);
        }
    }
};


int main()
{
    UTParseExpression tester("parseExpression");

    using LexemVector = std::vector<std::shared_ptr<LexemBase>>;

    tester.runTest("Usual string", {
        "'abaca2b:a", {
            ExpressionType::TEXT,
            LexemVector{std::make_shared<LexemText>("abaca2b:a")}
        }
    });
    tester.runTest("Empty string", {
        "'", {
            ExpressionType::TEXT,
            LexemVector{std::make_shared<LexemText>("")}
        }
    });
    tester.runTest("Usual string with number", {
        "'9323", {
            ExpressionType::TEXT,
            LexemVector{std::make_shared<LexemText>("9323")}
        }
    });
    tester.runTest("Usual string with '", {
        "''", {
            ExpressionType::TEXT,
            LexemVector{std::make_shared<LexemText>("'")}
        }
    });
    tester.runTest("Usual string with escaping", {
        R"('\'\n)", {
            ExpressionType::TEXT,
            LexemVector{std::make_shared<LexemText>(R"(\'\n)")}
        }
    });

    tester.runTest("Empty expression", {
        "", {
            ExpressionType::NONE
        }
    });

    tester.runTest("Usual number", {
        "280902", {
            ExpressionType::ARITHMETIC,
            LexemVector{std::make_shared<LexemNumber>(280902)}
        }
    });
    tester.runTest("Negative number", {
        "-2"
    });

    tester.runTest("Expression without =", {
        "9+13"
    });

    tester.runTest("Simple expression", {
        "=87", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemNumber>(87),
            }
        }
    });
    tester.runTest("Simple expression", {
        "=9-13", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemNumber>(9),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemNumber>(13),
            }
        }
    });

    tester.runTest("Big expression", {
        "=49+321-C9*0/A4-A1*76", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemNumber>(49),
                std::make_shared<LexemOperation>(Operation::ADD),
                std::make_shared<LexemNumber>(321),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemCellReference>(Coordinate2D(8, 2)),
                std::make_shared<LexemOperation>(Operation::MULTIPLY),
                std::make_shared<LexemNumber>(0),
                std::make_shared<LexemOperation>(Operation::DIVIDE),
                std::make_shared<LexemCellReference>(Coordinate2D(3, 0)),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemCellReference>(Coordinate2D(0, 0)),
                std::make_shared<LexemOperation>(Operation::MULTIPLY),
                std::make_shared<LexemNumber>(76),
            }
        }
    });

    tester.runTest("Simple expression with error, but valid before calculation", {
        "=-9-13", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemNumber>(9),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemNumber>(13),
            }
        }
    });
    tester.runTest("Simple expression with error, but valid before calculation", {
        "=+9-13", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemOperation>(Operation::ADD),
                std::make_shared<LexemNumber>(9),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemNumber>(13),
            }
        }
    });
    tester.runTest("Simple expression with excess space", {
        "= 9-13"
    });
    tester.runTest("Simple expression with error, but valid before calculation", {
        "=9--13", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemNumber>(9),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemNumber>(13),
            }
        }
    });
    tester.runTest("Simple expression with error, but valid before calculation", {
        "=9-+13", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemNumber>(9),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemOperation>(Operation::ADD),
                std::make_shared<LexemNumber>(13),
            }
        }
    });
    tester.runTest("Simple expression with error, but valid before calculation", {
        "=++/-*+", {
            ExpressionType::ARITHMETIC,
            LexemVector{
                std::make_shared<LexemOperation>(Operation::ADD),
                std::make_shared<LexemOperation>(Operation::ADD),
                std::make_shared<LexemOperation>(Operation::DIVIDE),
                std::make_shared<LexemOperation>(Operation::SUBTRACT),
                std::make_shared<LexemOperation>(Operation::MULTIPLY),
                std::make_shared<LexemOperation>(Operation::ADD),
            }
        }
    });
    tester.runTest("Simple expression with error", {
        "=A1A1"
    });

    return 0;
}
