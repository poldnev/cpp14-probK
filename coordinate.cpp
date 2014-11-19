#include <functional>

#include "coordinate.h"


Coordinate2D::Coordinate2D(int row_tmp, int column_tmp) : row(row_tmp), column(column_tmp) {}

bool Coordinate2D::operator<(const Coordinate2D &second) const {
    return
        row < second.row
        || (
            row == second.row
            && column < second.column
        );
}

bool Coordinate2D::operator==(const Coordinate2D &second) const {
    return row == second.row && column == second.column;
}


size_t Coordinate2DHash::operator()(const Coordinate2D &coordinate) const {
    return std::hash<int>()(coordinate.row) ^ (std::hash<int>()(coordinate.row) << 16);
}
