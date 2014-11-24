#include <sstream>
#include <map>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include <string>
#include <utility>

#include "utils.h"
#include "coordinate.h"
#include "sparse_table.h"


template <typename ValueType>
const ValueType SparseTable<ValueType>::EMPTY_CELL = {};


template <typename ValueType>
void SparseTable<ValueType>::assertCoordinateInRange(const Coordinate2D &coordinate) const {
    if (
        coordinate.row < 0 || coordinate.row >= height
        || coordinate.column < 0 || coordinate.column >= width
    ) {
        std::stringstream message_stream;
        message_stream << "Coordinates (" << coordinate.row << ", " << coordinate.column << ") ";
        message_stream << "are out of range (" << height << ", " << width << ")";
        throw std::out_of_range(message_stream.str());
    }
}


template <typename ValueType>
SparseTable<ValueType>::Entry::Entry(const Coordinate2D &coordinate_tmp, const ValueType &value_tmp)
    : coordinate(coordinate_tmp), value(value_tmp) {}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenCellIterator::isIteratorStrictlyHere() const {
    return
        next_iterator != end_iterator
        && row == next_iterator->first.row
        && column == next_iterator->first.column;
}

template <typename ValueType>
SparseTable<ValueType>::FlattenCellIterator::FlattenCellIterator(
    CellIterator next_iterator_tmp,
    CellIterator end_iterator_tmp,
    int row_tmp, int column_tmp
) : next_iterator(next_iterator_tmp), end_iterator(end_iterator_tmp), row(row_tmp), column(column_tmp) {}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenCellIterator &SparseTable<ValueType>::FlattenCellIterator::operator++() {
    if (isIteratorStrictlyHere()) {
        ++next_iterator;
    }
    ++column;
    return *this;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenCellIterator SparseTable<ValueType>::FlattenCellIterator::operator++(int) {
    auto iterator_copy = *this;
    ++*this;
    return iterator_copy;
}

template <typename ValueType>
const ValueType &SparseTable<ValueType>::FlattenCellIterator::operator*() const {
    if (isIteratorStrictlyHere()) {
        return next_iterator->second;
    } else {
        return SparseTable::EMPTY_CELL;
    }
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenCellIterator::operator==(const FlattenCellIterator &other) const {
    return next_iterator == other.next_iterator && row == other.row && column == other.column;
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenCellIterator::operator!=(const FlattenCellIterator &other) const {
    return !operator==(other);
}

template <typename ValueType>
SparseTable<ValueType>::FlattenRow::FlattenRow(const SparseTable &table_tmp, int row_tmp) : table(table_tmp), row(row_tmp) {}

template <typename ValueType>
const auto &SparseTable<ValueType>::FlattenRow::operator[](const int column) const {
    return table(row, column);
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenCellIterator SparseTable<ValueType>::FlattenRow::begin() const {
    return {table.table_values.lower_bound({row, 0}), table.table_values.end(), row, 0};
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenCellIterator SparseTable<ValueType>::FlattenRow::end() const {
    return {table.table_values.lower_bound({row + 1, 0}), table.table_values.end(), row, table.width};
}

template <typename ValueType>
SparseTable<ValueType>::FlattenRowIterator::FlattenRowIterator(const SparseTable &table_tmp, const int row_tmp) : table(table_tmp), row(row_tmp) {}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRow SparseTable<ValueType>::FlattenRowIterator::operator*() const {
    return {table, row};
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator &SparseTable<ValueType>::FlattenRowIterator::operator++() {
    ++row;
    return *this;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator SparseTable<ValueType>::FlattenRowIterator::operator++(int) {
    auto iterator_copy = *this;
    ++*this;
    return iterator_copy;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator &SparseTable<ValueType>::FlattenRowIterator::operator--() {
    --row;
    return *this;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator SparseTable<ValueType>::FlattenRowIterator::operator--(int) {
    auto iterator_copy = *this;
    --*this;
    return iterator_copy;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator &SparseTable<ValueType>::FlattenRowIterator::operator+=(
    int delta
) {
    row += delta;
    return *this;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator &SparseTable<ValueType>::FlattenRowIterator::operator-=(
    int delta
) {
    row -= delta;
    return *this;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator SparseTable<ValueType>::FlattenRowIterator::operator+(
    int delta
) const {
   return {table, row + delta};
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator operator+(
    int delta,
    const typename SparseTable<ValueType>::FlattenRowIterator &iterator
) {
   return iterator + delta;
}

template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator SparseTable<ValueType>::FlattenRowIterator::operator-(
    int delta
) const {
   return {table, row - delta};
}

template <typename ValueType>
int SparseTable<ValueType>::FlattenRowIterator::operator-(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
   return row - other.row;
}

template <typename ValueType>
auto SparseTable<ValueType>::FlattenRowIterator::operator[](int delta) const {
   return *(*this + delta);
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenRowIterator::operator==(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
    return &table == &other.table && row == other.row;
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenRowIterator::operator!=(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
    return !operator==(other);
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenRowIterator::operator<(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
    return row < other.row;
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenRowIterator::operator>(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
    return other < *this;
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenRowIterator::operator<=(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
    return !(*this > other);
}

template <typename ValueType>
bool SparseTable<ValueType>::FlattenRowIterator::operator>=(
    const SparseTable<ValueType>::FlattenRowIterator &other
) const {
    return !(*this < other);
}


template <typename ValueType>
SparseTable<ValueType>::SparseTable(const int height_tmp, const int width_tmp) : height(height_tmp), width(width_tmp) {}

template <typename ValueType>
int SparseTable<ValueType>::getHeight() const {
    return height;
}

template <typename ValueType>
int SparseTable<ValueType>::getWidth() const {
    return width;
}

template <typename ValueType>
bool SparseTable<ValueType>::operator==(const SparseTable<ValueType> &other) const {
    return
        height == other.height
        && width == other.width
        && table_values == other.table_values;
}

template <typename ValueType>
template <typename... CoordinateArgs>
ValueType &SparseTable<ValueType>::operator()(CoordinateArgs&&... coordinate_args) {
    Coordinate2D coordinate(std::forward<CoordinateArgs>(coordinate_args)...);
    assertCoordinateInRange(coordinate);
    return table_values[coordinate];
}

template <typename ValueType>
template <typename... CoordinateArgs>
const ValueType &SparseTable<ValueType>::operator()(CoordinateArgs&&... coordinate_args) const {
    auto non_const_pointer = const_cast<SparseTable*>(this);
    return (*non_const_pointer)(std::forward<CoordinateArgs>(coordinate_args)...);
}

template <typename ValueType>
int SparseTable<ValueType>::getElementCount() const {
    return table_values.size();
}

template <typename ValueType>
IteratorRange<typename SparseTable<ValueType>::CellIterator> SparseTable<ValueType>::getElements() const {
    return {table_values.cbegin(), table_values.cend()};
}

template <typename ValueType>
IteratorRange<typename SparseTable<ValueType>::FlattenRowIterator> SparseTable<ValueType>::flatten() const {
    return {{*this, 0}, {*this, height}};
}

template <typename InputIterator>
SparseTable<typename std::iterator_traits<InputIterator>::value_type::value_type>
makeSparseTable(int height, int width, InputIterator first, InputIterator last) {
    using ValueType = typename std::iterator_traits<InputIterator>::value_type::value_type;
    SparseTable<ValueType> table(height, width);
    for (const auto &entry : IteratorRange<InputIterator>(first, last)) {
        table(entry.coordinate) = entry.value;
    }
    return table;
}

