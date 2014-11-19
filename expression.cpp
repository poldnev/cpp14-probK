#include <vector>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <cctype>
#include <string>
#include <functional>
#include <algorithm>

#include "utils.h"
#include "expression.h"


LexemBase::LexemBase() {}

LexemBase::~LexemBase() {}


LexemText::LexemText(const std::string &text_tmp) : text(text_tmp) {}

LexemText::LexemText(std::string &&text_tmp) : text(std::move(text_tmp)) {}

LexemType LexemText::getType() const {
    return LexemType::TEXT;
}

const std::string &LexemText::getText() const {
    return text;
}

bool LexemText::operator==(const LexemText &other) const {
    return text == other.text;
}

bool LexemText::equalTo(const LexemBase *other) const {
    return
        getType() == other->getType()
        && *this == *dynamic_cast<const LexemText*>(other);
}

LexemText::~LexemText() {}


LexemNumber::LexemNumber(int number_tmp) : number(number_tmp) {}

LexemType LexemNumber::getType() const {
    return LexemType::NUMBER;
}

int LexemNumber::getNumber() const {
    return number;
}

std::string LexemNumber::getText() const {
    std::stringstream number_stream;
    number_stream << number;
    return number_stream.str();
}

bool LexemNumber::operator==(const LexemNumber &other) const {
    return number == other.number;
}

bool LexemNumber::equalTo(const LexemBase *other) const {
    return
        getType() == other->getType()
        && *this == *dynamic_cast<const LexemNumber*>(other);
}

LexemNumber::~LexemNumber() {}


int parseNumber(const std::string &raw_number) {
    if (
        raw_number.empty()
        || std::count_if(raw_number.begin(), raw_number.end(), [](char character){ return !std::isdigit(character); }) > 0
    ) {
        std::stringstream message_stream;
        message_stream << "'" << raw_number << "' is not a non-negative integer number";
        throw std::invalid_argument(message_stream.str());
    }

    std::stringstream number_stream;
    number_stream << raw_number;
    int number;
    number_stream >> number;
    return number;
}


LexemCellReference::LexemCellReference(const Coordinate2D &coordinate_tmp) : coordinate(coordinate_tmp) {}

LexemCellReference::LexemCellReference(Coordinate2D &&coordinate_tmp) : coordinate(std::move(coordinate_tmp)) {}

LexemType LexemCellReference::getType() const {
    return LexemType::CELL_REFERENCE;
}

const Coordinate2D &LexemCellReference::getCoordinate() const {
    return coordinate;
}

bool LexemCellReference::operator==(const LexemCellReference &other) const {
    return coordinate == other.coordinate;
}

bool LexemCellReference::equalTo(const LexemBase *other) const {
    return
        getType() == other->getType()
        && *this == *dynamic_cast<const LexemCellReference*>(other);
}

LexemCellReference::~LexemCellReference() {}


Coordinate2D parseCoordinate(const std::string &raw_coordinate) {
    if (raw_coordinate.length() != 2 || !isalpha(raw_coordinate[0]) || !isdigit(raw_coordinate[1]) || raw_coordinate[1] == '0') {
        std::stringstream message_stream;
        message_stream << "Coordinate '" << raw_coordinate << "' is ill-formed";
        throw std::invalid_argument(message_stream.str());
    }

    char row_name = raw_coordinate[1];
    char column_name = raw_coordinate[0];

    return {row_name - '1', std::tolower(column_name) - 'a'};
}


LexemOperation::LexemOperation(const Operation &operation_tmp) : operation(operation_tmp) {}

LexemOperation::LexemOperation(Operation &&operation_tmp) : operation(std::move(operation_tmp)) {}

LexemType LexemOperation::getType() const {
    return LexemType::OPERATION;
}

const Operation &LexemOperation::getOperation() const {
    return operation;
}

std::function<int(int, int)> LexemOperation::getAction() const {
    if (operation == Operation::ADD) {
        return std::plus<int>();
    } else if (operation == Operation::SUBTRACT) {
        return std::minus<int>();
    } else if (operation == Operation::MULTIPLY) {
        return std::multiplies<int>();
    } else if (operation == Operation::DIVIDE) {
        return [](int dividend, int divisor) {
            if (divisor == 0) {
                throw std::domain_error("Division by 0");
            } else {
                return dividend / divisor;
            }
        };
    }
    throw std::invalid_argument("Unimplemented operation");
}

bool LexemOperation::operator==(const LexemOperation &other) const {
    return operation == other.operation;
}

bool LexemOperation::equalTo(const LexemBase *other) const {
    return
        getType() == other->getType()
        && *this == *dynamic_cast<const LexemOperation*>(other);
}

LexemOperation::~LexemOperation() {}


Operation parseOperation(const std::string &raw_operation) {
    if (raw_operation == "+") {
        return Operation::ADD;
    } else if (raw_operation == "-") {
        return Operation::SUBTRACT;
    } else if (raw_operation == "*") {
        return Operation::MULTIPLY;
    } else if (raw_operation == "/") {
        return Operation::DIVIDE;
    } else {
        std::stringstream message_stream;
        message_stream << "Operation '" << raw_operation << "' is unknown";
        throw std::invalid_argument(message_stream.str());
    }
}


Expression::Expression(ExpressionType type_tmp) : type(type_tmp) {}

ExpressionType Expression::getType() const {
    return type;
}

size_t Expression::getSize() const {
    return lexems.size();
}

void Expression::pushLexem(std::shared_ptr<LexemBase> lexem_pointer) {
    lexems.push_back(lexem_pointer);
}

typename Expression::LexemIterator Expression::begin() {
    return lexems.begin();
}

typename Expression::LexemIterator Expression::end() {
    return lexems.end();
}

typename Expression::LexemConstIterator Expression::begin() const {
    return lexems.begin();
}

typename Expression::LexemConstIterator Expression::end() const {
    return lexems.end();
}


std::shared_ptr<LexemBase> makeLexemPointer(LexemType lexem_type, const std::string &raw_lexem) {
    if (lexem_type == LexemType::TEXT) {
        return std::make_shared<LexemText>(raw_lexem);
    } else if (lexem_type == LexemType::NUMBER) {
        return std::make_shared<LexemNumber>(parseNumber(raw_lexem));
    } else if (lexem_type == LexemType::CELL_REFERENCE) {
        return std::make_shared<LexemCellReference>(parseCoordinate(raw_lexem));
    } else if (lexem_type == LexemType::OPERATION) {
        return std::make_shared<LexemOperation>(parseOperation(raw_lexem));
    }
    throw std::invalid_argument("Invalid lexem type");
}


Expression makeErrorExpression(const std::string &error_message) {
    Expression expression(ExpressionType::ERROR);
    expression.pushLexem(makeLexemPointer(LexemType::TEXT, makeErrorMessage(error_message)));
    return expression;
}


Expression parseExpression(const std::string &raw_expression) {
    if (raw_expression.empty()) {
        return {};
    }

    try {
        if (raw_expression[0] == '\'') {

            Expression expression(ExpressionType::TEXT);
            expression.pushLexem(makeLexemPointer(LexemType::TEXT, raw_expression.substr(1)));
            return expression;

        } else if (raw_expression[0] == '=') {

            Expression expression(ExpressionType::ARITHMETIC);
            LexemType lexem_type = LexemType::NUMBER;
            std::string raw_lexem;
            for (auto it = raw_expression.cbegin() + 1; it != raw_expression.cend(); ++it) {
                char character = *it;
                if (std::isalnum(character)) {
                    if (raw_lexem.empty()) {
                        lexem_type = (std::isdigit(character) ? LexemType::NUMBER : LexemType::CELL_REFERENCE);
                    }
                    raw_lexem.push_back(character);
                } else {
                    if (!raw_lexem.empty()) {
                        expression.pushLexem(makeLexemPointer(lexem_type, raw_lexem));
                        raw_lexem.clear();
                    }
                    expression.pushLexem(makeLexemPointer(LexemType::OPERATION, std::string(1, character)));
                }
            }
            if (!raw_lexem.empty()) {
                expression.pushLexem(makeLexemPointer(lexem_type, raw_lexem));
            }
            return expression;

        } else {

            Expression expression(ExpressionType::ARITHMETIC);
            expression.pushLexem(makeLexemPointer(LexemType::NUMBER, raw_expression));
            return expression;

        }

    } catch (const std::exception &exception) {
        return makeErrorExpression(exception.what());
    }
}
