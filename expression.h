#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED


#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "coordinate.h"



enum class ExpressionType {
    NONE,
    TEXT,
    ARITHMETIC,
    ERROR
};

enum class LexemType {
    TEXT,
    NUMBER,
    CELL_REFERENCE,
    OPERATION
};

enum class Operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};


class LexemBase {
public:
    LexemBase();
    virtual LexemType getType() const = 0;
    virtual bool equalTo(const LexemBase *other) const = 0;
    virtual ~LexemBase();
};


class LexemText : public LexemBase {
    std::string text;
public:
    LexemText(const std::string &text_tmp);
    LexemText(std::string &&text_tmp);
    LexemType getType() const;
    const std::string &getText() const;
    bool operator==(const LexemText &other) const;
    bool equalTo(const LexemBase *other) const;
    ~LexemText();
};


class LexemNumber : public LexemBase {
    int number;
public:
    LexemNumber(int number_tmp);
    LexemType getType() const;
    int getNumber() const;
    std::string getText() const;
    bool operator==(const LexemNumber &other) const;
    bool equalTo(const LexemBase *other) const;
    ~LexemNumber();
};

int parseNumber(const std::string &raw_number);


class LexemCellReference : public LexemBase {
    Coordinate2D coordinate;
public:
    LexemCellReference(const Coordinate2D &coordinate_tmp);
    LexemCellReference(Coordinate2D &&coordinate_tmp);
    LexemType getType() const;
    const Coordinate2D &getCoordinate() const;
    bool operator==(const LexemCellReference &other) const;
    bool equalTo(const LexemBase *other) const;
    ~LexemCellReference();
};

Coordinate2D parseCoordinate(const std::string &raw_coordinate);


class LexemOperation : public LexemBase {
    Operation operation;
public:
    LexemOperation(const Operation &operation_tmp);
    LexemOperation(Operation &&operation_tmp);
    LexemType getType() const;
    const Operation &getOperation() const;
    std::function<int(int, int)> getAction() const;
    bool operator==(const LexemOperation &other) const;
    bool equalTo(const LexemBase *other) const;
    ~LexemOperation();
};

Operation parseOperation(const std::string &raw_operation);


class Expression
{
    ExpressionType type;
    std::vector<std::shared_ptr<LexemBase>> lexems;
    using LexemIterator = std::vector<std::shared_ptr<LexemBase>>::iterator;
    using LexemConstIterator = std::vector<std::shared_ptr<LexemBase>>::const_iterator;
public:
    Expression(ExpressionType type_tmp = ExpressionType::NONE);

    // Construct expression with given type and args of lexems constructor
    template <typename... LexemsArgs>
    Expression(ExpressionType type_tmp, LexemsArgs&&... lexems_args);

    ExpressionType getType() const;
    size_t getSize() const;
    void pushLexem(std::shared_ptr<LexemBase> lexem_pointer);
    LexemIterator begin();
    LexemIterator end();
    LexemConstIterator begin() const;
    LexemConstIterator end() const;
    bool operator==(const Expression &other) const;
};


// Parse lexem of given type from given string and return pointer to it
std::shared_ptr<LexemBase> makeLexemPointer(LexemType lexem_type, const std::string &raw_lexem);

Expression makeErrorExpression(const std::string &error_message);

Expression parseExpression(const std::string &raw_expression);

int calculateArithmeticExpression(const Expression &expression);



template <typename... LexemsArgs>
Expression::Expression(ExpressionType type_tmp, LexemsArgs&&... lexems_args)
    : type(type_tmp), lexems(std::forward<LexemsArgs>(lexems_args)...) {}


#endif // EXPRESSION_H_INCLUDED
