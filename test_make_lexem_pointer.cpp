#include <memory>

#include "unit_test.h"
#include "unit_test.cpp"
#include "coordinate.h"
#include "expression.h"


struct MakeLexemPointerTest
{
    std::string raw_lexem;
    std::shared_ptr<LexemBase> lexem_pointer;
    LexemType lexem_type;

    // success test
    MakeLexemPointerTest(const std::string &raw_lexem_tmp, const std::shared_ptr<LexemBase> &lexem_pointer_tmp)
        : raw_lexem(raw_lexem_tmp), lexem_pointer(lexem_pointer_tmp), lexem_type(lexem_pointer_tmp->getType()) {}

    // failure test
    MakeLexemPointerTest(const std::string &raw_lexem_tmp, LexemType lexem_type_tmp)
        : raw_lexem(raw_lexem_tmp), lexem_pointer(), lexem_type(lexem_type_tmp) {}

    bool isError() const {
        return lexem_pointer == nullptr;
    }
};


class UTMakeLexemPointer : public UnitTester<MakeLexemPointerTest>
{
public:
    UTMakeLexemPointer(const std::string &plan_name) : UnitTester(plan_name) {}

    bool checkTest(const MakeLexemPointerTest &test) const {
        try {
            const auto &expected = test.lexem_pointer;
            auto got = makeLexemPointer(test.lexem_type, test.raw_lexem);
            return expected && expected->equalTo(got.get());
        } catch (const std::exception&) {
            return test.isError();
        }
    }
};


int main()
{
    UTMakeLexemPointer tester("makeLexemPointer");

    tester.runTest("Usual string", {
        "'abaca2b:a", std::make_shared<LexemText>("'abaca2b:a")
    });
    tester.runTest("Empty string", {
        "", std::make_shared<LexemText>("")
    });

    tester.runTest("Usual number", {
        "980209", std::make_shared<LexemNumber>(980209)
    });
    tester.runTest("Usual number", {
        "219", std::make_shared<LexemNumber>(219)
    });
    tester.runTest("Usual number starting with 0", {
        "054", std::make_shared<LexemNumber>(54)
    });
    tester.runTest("Zero", {
        "0", std::make_shared<LexemNumber>(0)
    });
    tester.runTest("Minus zero", {
        "-0", LexemType::NUMBER
    });
    tester.runTest("Negative number", {
        "-912", LexemType::NUMBER
    });
    tester.runTest("Positive number with +", {
        "+219", LexemType::NUMBER
    });
    tester.runTest("Real number", {
        "3.14", LexemType::NUMBER
    });
    tester.runTest("Not a number: empty string", {
        "", LexemType::NUMBER
    });
    tester.runTest("Not a number: letter at the end", {
        "949a", LexemType::NUMBER
    });
    tester.runTest("Not a number: trailing space", {
        "949 ", LexemType::NUMBER
    });
    tester.runTest("Not a number: leading space", {
        " 949", LexemType::NUMBER
    });
    tester.runTest("Not a number: text", {
        "NaN", LexemType::NUMBER
    });

    tester.runTest("Cell reference A1", {
        "A1", std::make_shared<LexemCellReference>(Coordinate2D(0, 0))
    });
    tester.runTest("Cell reference A9", {
        "A9", std::make_shared<LexemCellReference>(Coordinate2D(8, 0))
    });
    tester.runTest("Cell reference Z9", {
        "Z9", std::make_shared<LexemCellReference>(Coordinate2D(8, 25))
    });
    tester.runTest("Cell reference Z1", {
        "Z1", std::make_shared<LexemCellReference>(Coordinate2D(0, 25))
    });
    tester.runTest("Cell reference D6", {
        "D6", std::make_shared<LexemCellReference>(Coordinate2D(5, 3))
    });
    tester.runTest("Cell reference F0", {
        "F0", LexemType::CELL_REFERENCE
    });
    tester.runTest("Cell reference AD9", {
        "AD9", LexemType::CELL_REFERENCE
    });
    tester.runTest("Cell reference A-3", {
        "A-3", LexemType::CELL_REFERENCE
    });
    tester.runTest("Not a cell reference", {
        "CD", LexemType::CELL_REFERENCE
    });
    tester.runTest("Not a cell reference", {
        "B", LexemType::CELL_REFERENCE
    });
    tester.runTest("Not a cell reference", {
        "9", LexemType::CELL_REFERENCE
    });
    tester.runTest("Not a cell reference", {
        "", LexemType::CELL_REFERENCE
    });

    tester.runTest("Operation +", {
        "+", std::make_shared<LexemOperation>(Operation::ADD)
    });
    tester.runTest("Operation -", {
        "-", std::make_shared<LexemOperation>(Operation::SUBTRACT)
    });
    tester.runTest("Operation *", {
        "*", std::make_shared<LexemOperation>(Operation::MULTIPLY)
    });
    tester.runTest("Operation /", {
        "/", std::make_shared<LexemOperation>(Operation::DIVIDE)
    });
    tester.runTest("Not an operation", {
        "", LexemType::OPERATION
    });
    tester.runTest("Not an operation", {
        "++", LexemType::OPERATION
    });
    tester.runTest("Not an operation", {
        "%", LexemType::OPERATION
    });
    tester.runTest("Not an operation", {
        "0", LexemType::OPERATION
    });
    tester.runTest("Not an operation", {
        "=", LexemType::OPERATION
    });
    tester.runTest("Not an operation", {
        "plus", LexemType::OPERATION
    });

    return 0;
}
