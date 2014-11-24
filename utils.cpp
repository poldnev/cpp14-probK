#include <string>
#include <istream>
#include <cstdio>

#include "utils.h"


std::string makeErrorMessage(const std::string &error_message) {
    return "#" + error_message;
}

std::istream& getline(std::istream& input_stream, std::string& string)
{
    string.clear();

    std::istream::sentry sentry(input_stream, true);
    std::streambuf* streambuf = input_stream.rdbuf();

    while (true) {
        int character = streambuf->sbumpc();
        switch (character) {
        case '\n':
            return input_stream;
        case '\r':
            if (streambuf->sgetc() == '\n') {
                streambuf->sbumpc();
            }
            return input_stream;
        case EOF:
            // Also handle the case when the last line has no line ending
            if (string.empty()) {
                input_stream.setstate(std::ios::eofbit);
            }
            return input_stream;
        default:
            string.push_back(static_cast<char>(character));
        }
    }
}
