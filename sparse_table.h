#ifndef SPARSE_TABLE_H_INCLUDED
#define SPARSE_TABLE_H_INCLUDED

#include <map>
#include <iterator>
#include <iostream>
#include <string>

#include "utils.h"
#include "coordinate.h"


template <typename ValueType>
class SparseTable
{
    int height;
    int width;
    std::map<Coordinate2D, ValueType> table_values;

    static const ValueType EMPTY_CELL;

    using CellIterator = typename std::map<Coordinate2D, ValueType>::const_iterator;

    void assertCoordinateInRange(const Coordinate2D &coordinate) const;

    class FlattenCellIterator : std::iterator<std::forward_iterator_tag, ValueType, int>
    {
        // column of next_iterator should be >= column
        typename std::map<Coordinate2D, ValueType>::const_iterator next_iterator;
        int row;
        int column;

        bool isIteratorStrictlyHere() const;
    public:
        FlattenCellIterator(typename std::map<Coordinate2D, ValueType>::const_iterator next_iterator_tmp, int row_tmp, int column_tmp);
        FlattenCellIterator &operator++();
        FlattenCellIterator operator++(int);
        const ValueType &operator*() const;
        bool operator==(const FlattenCellIterator &other) const;
        bool operator!=(const FlattenCellIterator &other) const;
    };

    class FlattenRow
    {
        const SparseTable &table;
        int row;
    public:
        FlattenRow(const SparseTable &table_tmp, int row_tmp);
        const auto &operator[](const int column) const;
        FlattenCellIterator begin() const;
        FlattenCellIterator end() const;
    };

    class FlattenRowIterator : std::iterator<std::random_access_iterator_tag, FlattenRow, int>
    {
        const SparseTable &table;
        int row;
    public:
        FlattenRowIterator(const SparseTable &table_tmp, const int row_tmp);
        FlattenRow operator*() const;
        FlattenRowIterator &operator++();
        FlattenRowIterator operator++(int);
        FlattenRowIterator &operator--();
        FlattenRowIterator operator--(int);
        FlattenRowIterator &operator+=(int delta);
        FlattenRowIterator &operator-=(int delta);
        FlattenRowIterator operator+(int delta) const;
        FlattenRowIterator operator-(int delta) const;
        int operator-(const FlattenRowIterator &other) const;
        auto operator[](int delta) const;
        bool operator==(const FlattenRowIterator &other) const;
        bool operator!=(const FlattenRowIterator &other) const;
        bool operator<(const FlattenRowIterator &other) const;
        bool operator>(const FlattenRowIterator &other) const;
        bool operator<=(const FlattenRowIterator &other) const;
        bool operator>=(const FlattenRowIterator &other) const;
    };

public:
    SparseTable(const int height_tmp = 0, const int width_tmp = 0);

    int getHeight() const;
    int getWidth() const;

    // Access to cell with coordinates constructed with given arguments
    template <typename... CoordinateArgs>
    ValueType &operator()(CoordinateArgs&&... coordinate_args);

    template <typename... CoordinateArgs>
    const ValueType &operator()(CoordinateArgs&&... coordinate_args) const;

    int getElementCount() const;

    IteratorRange<CellIterator> getElements() const;

    // Allows to iterate over all cells including empty ones
    IteratorRange<FlattenRowIterator> flatten() const;
};


template <typename ValueType>
typename SparseTable<ValueType>::FlattenRowIterator operator+(
    int delta,
    const typename SparseTable<ValueType>::FlattenRowIterator &iterator
);


#endif // SPARSE_TABLE_H_INCLUDED
