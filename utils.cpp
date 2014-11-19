#include <string>

#include "utils.h"


std::string makeErrorMessage(const std::string &error_message) {
    return "#" + error_message;
}
