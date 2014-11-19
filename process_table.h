#ifndef PROCESS_TABLE_H_INCLUDED
#define PROCESS_TABLE_H_INCLUDED

#include <string>
#include <iostream>

#include "sparse_table.h"
#include "expression.h"


using TextTable = SparseTable<std::string>;


TextTable readTextTable(std::istream &in_stream = std::cin);


using ExpressionTable = SparseTable<Expression>;


ExpressionTable parseRawTable(const TextTable &raw_table);


int getProcessedArithmeticExpressionValue(const Expression &expression);


// One step of DFS: calculates value of given cell
void calculateCell(ExpressionTable &table, std::set<Coordinate2D> &visited_coordinates, const Coordinate2D &coordinate);


// Calculates values of all arithmetic expressions in table
void calculateParsedTable(ExpressionTable &table);


TextTable makePrintedTable(const ExpressionTable &table);


void printTextTable(const TextTable &table, std::ostream &out_stream = std::cout);


#endif // PROCESS_TABLE_H_INCLUDED
