#include <iostream>
#include <cstring>

bool check_if_is_array(const char* str) {
    if (!str) {
        return false;
    }

    int length = strlen(str);

    if (length < 2) {
        return false;
    }

    return (str[0] == '[' && str[length - 1] == ']');
}