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
#include "logger.h"


int main() {

    try {

        auto raw_table = readTextTable();

        auto parsed_table = parseRawTable(raw_table);
        calculateParsedTable(parsed_table);
        auto printed_table = makePrintedTable(parsed_table);

        printTextTable(printed_table);

    }  catch (const std::exception &exception) {

        log_error(exception.what());
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}
