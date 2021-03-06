#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <algorithm>
#include <string>
#include <functional>

#include "expression_table.h"
#include "sparse_table.h"
#include "sparse_table.cpp"
#include "expression.h"
#include "logger.h"



TextTable readTextTable(std::istream &in_stream) {
    int height, width;

    if (!(in_stream >> height >> width)) {
        throw std::invalid_argument("First input line should contain two integers: table height and width");
    }

    if (height < 0 || width < 0) {
        throw std::invalid_argument("Table height and width should be positive integers");
    }
    if (height == 0 || width == 0) {
        log_warn("Table height or width is 0, do nothing");
        return {};
    }

    TextTable table(height, width);
    std::string temp;

    if (!getline(in_stream, temp)) {
        log_warn("Table has 0 rows instead of ", height);
        return table;
    }
    if (!temp.empty()) {
        log_warn("Excess information in first line, ignore it");
    }

    for (int row_index = 0; row_index < height; ++row_index) {
        std::string raw_row;

        if (!getline(in_stream, raw_row) && row_index < height - 1) {
            log_warn("Table has ", row_index + 1, " rows instead of ", height);
            break;
        }

        std::stringstream row_stream(raw_row);

        for (int column_index = 0; column_index < width; ++column_index) {
            std::string cell;

            if (!std::getline(row_stream, cell, '\t') && column_index < width - 1) {
                log_warn("Row ", row_index + 1, " has ", column_index + 1, " cells instead of ", width);
                break;
            }
            if (!cell.empty()) {
                table(row_index, column_index) = cell;
            }
        }
    }

    return table;
}


ExpressionTable parseRawTable(const TextTable &raw_table) {
    ExpressionTable parsed_table(raw_table.getHeight(), raw_table.getWidth());
    for (const auto &cell_pair : raw_table.getElements()) {
        parsed_table(cell_pair.first) = parseExpression(cell_pair.second);
    }
    return parsed_table;
}


int getProcessedArithmeticExpressionValue(const Expression &expression) {
    if (expression.getType() == ExpressionType::ERROR) {
        throw std::invalid_argument("Error in referred cell");
    } else if (expression.getType() != ExpressionType::ARITHMETIC) {
        throw std::invalid_argument("Not a number in referred cell");
    } else if (expression.getSize() != 1 || (*expression.begin())->getType() != LexemType::NUMBER) {
        throw std::logic_error("Infinite cycle in references");
    } else {
        return dynamic_cast<LexemNumber*>(expression.begin()->get())->getNumber();
    }
}


void calculateCell(ExpressionTable &table, std::set<Coordinate2D> &visited_coordinates, const Coordinate2D &coordinate) {
    visited_coordinates.insert(coordinate);
    Expression &current_expression = table(coordinate);

    try {

        Expression new_expression;
        for (const auto &lexem_pointer : current_expression) {
            if (lexem_pointer->getType() == LexemType::CELL_REFERENCE) {

                auto referred_coordinate = dynamic_cast<LexemCellReference*>(lexem_pointer.get())->getCoordinate();
                const auto &referred_expression = table(referred_coordinate);
                if (visited_coordinates.count(referred_coordinate) == 0 && referred_expression.getType() == ExpressionType::ARITHMETIC) {
                    calculateCell(table, visited_coordinates, referred_coordinate);
                }
                new_expression.pushLexem(std::make_shared<LexemNumber>(getProcessedArithmeticExpressionValue(referred_expression)));

            } else {

                new_expression.pushLexem(lexem_pointer);

            }
        }

        current_expression = Expression(ExpressionType::ARITHMETIC);
        current_expression.pushLexem(std::make_shared<LexemNumber>(calculateArithmeticExpression(new_expression)));

    } catch (const std::exception &exception) {
        current_expression = makeErrorExpression(exception.what());
    }
}


void calculateParsedTable(ExpressionTable &table) {
    std::set<Coordinate2D> visited_coordinates;

    for (const auto &cell_pair : table.getElements()) {
        const auto &coordinate = cell_pair.first;
        const auto &expression = cell_pair.second;
        if (visited_coordinates.count(coordinate)) {
            continue;
        }
        if (expression.getType() == ExpressionType::ARITHMETIC) {
            calculateCell(table, visited_coordinates, coordinate);
        }
    }
}


TextTable makePrintedTable(const ExpressionTable &table) {
    TextTable printed_table(table.getHeight(), table.getWidth());

    for (const auto &cell_pair : table.getElements()) {
        const auto &coordinate = cell_pair.first;
        const auto &expression = cell_pair.second;
        if (expression.getType() == ExpressionType::NONE) {
        } else if (expression.getType() == ExpressionType::TEXT || expression.getType() == ExpressionType::ERROR) {
            printed_table(coordinate) = dynamic_cast<LexemText*>(expression.begin()->get())->getText();
        } else if (expression.getType() == ExpressionType::ARITHMETIC && expression.getSize() == 1) {
            printed_table(coordinate) = dynamic_cast<LexemNumber*>(expression.begin()->get())->getText();
        } else {
            printed_table(coordinate) = makeErrorMessage("Illegal expression");
        }
    }

    return printed_table;
}


void printTextTable(const TextTable &table, std::ostream &out_stream) {
    for (const auto &row : table.flatten()) {
        int column_index = 0;
        for (const auto &cell : row) {
            out_stream << cell << (++column_index == table.getWidth() ? '\n' : '\t');
        }
    }
}
