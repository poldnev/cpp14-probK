#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>


template <typename Iterator>
class IteratorRange
{
    Iterator begin_iterator;
    Iterator end_iterator;
public:
    IteratorRange(Iterator begin_tmp, Iterator end_tmp) : begin_iterator(begin_tmp), end_iterator(end_tmp) {}

    Iterator begin() const {
        return begin_iterator;
    }

    Iterator end() const {
        return end_iterator;
    }
};


std::string makeErrorMessage(const std::string &error_message);

std::istream& getline(std::istream& input_stream, std::string& string);



#endif // UTILS_H_INCLUDED
