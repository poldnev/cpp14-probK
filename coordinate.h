#ifndef COORDINATE_H_INCLUDED
#define COORDINATE_H_INCLUDED


// Helper struct for using as index in SparseTable
struct Coordinate2D
{
    int row;
    int column;

    Coordinate2D(int row_tmp = 0, int column_tmp = 0);

    bool operator<(const Coordinate2D &second) const;
    bool operator==(const Coordinate2D &second) const;
};


struct Coordinate2DHash
{
    size_t operator()(const Coordinate2D &coordinate) const;
};


#endif // COORDINATE_H_INCLUDED
