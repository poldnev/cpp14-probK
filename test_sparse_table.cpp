#include <vector>
#include <unordered_map>

#include "unit_test.h"
#include "unit_test.cpp"
#include "coordinate.h"
#include "sparse_table.h"
#include "sparse_table.cpp"


struct SparseTableFlattenTestEntry
{
    Coordinate2D coordinate;
    std::string value;

    SparseTableFlattenTestEntry(const Coordinate2D &coordinate_tmp, const std::string &value_tmp)
        : coordinate(coordinate_tmp), value(value_tmp) {}
};


struct SparseTableFlattenTest
{
    int height;
    int width;
    std::vector<SparseTableFlattenTestEntry> entries;

    SparseTableFlattenTest(int height_tmp, int width_tmp, const std::vector<SparseTableFlattenTestEntry> &entries_tmp)
        : height(height_tmp), width(width_tmp), entries(entries_tmp) {}
};


class UTSparseTableFlatten : public UnitTester<SparseTableFlattenTest>
{
public:
    UTSparseTableFlatten(const std::string &plan_name) : UnitTester(plan_name) {}

    bool checkTest(const SparseTableFlattenTest &test) const {

        std::unordered_map<Coordinate2D, std::string, Coordinate2DHash> value_map;

        SparseTable<std::string> table(test.height, test.width);
        for (const auto &entry : test.entries) {
            table(entry.coordinate) = entry.value;
            value_map[entry.coordinate] = entry.value;
        }

        int row_index = 0;
        for (const auto &row : table.flatten()) {
            int column_index = 0;
            for (const auto &cell : row) {
                auto iterator = value_map.find({row_index, column_index});
                std::string expected = (iterator == value_map.end() ? "" : iterator->second);
                if (cell != expected) {
                    return false;
                }
                ++column_index;
            }
            ++row_index;
        }

        return true;

    }
};


int main()
{
    UTSparseTableFlatten tester("SparseTable::flatten");

    tester.runTest("One cell", {
        1, 1, {
            {{0, 0}, "A"},
        }
    });

    tester.runTest("3x1 with empty center", {
        3, 1, {
            {{2, 0}, "A"},
            {{0, 0}, "Y"},
        }
    });

    tester.runTest("3x3 with one filled cell", {
        3, 3, {
            {{1, 1}, "Center"},
        }
    });

    return 0;
}
