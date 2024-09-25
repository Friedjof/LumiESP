#include "utils.h"


bool isHexColor(String color) {
    // Check if the string starts with '#' and is either 4 or 7 characters long
    if ((color.length() != 4 && color.length() != 7) || color[0] != '#') {
        return false;
    }

    // Check if all characters after '#' are valid hexadecimal digits
    for (size_t i = 1; i < color.length(); ++i) {
        if (!std::isxdigit(color[i])) {
            return false;
        }
    }

    return true;
}