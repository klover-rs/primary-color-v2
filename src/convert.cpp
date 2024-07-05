#include <node_api.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "convert.h"
#include <cctype>  


const char* jsStringToCString(napi_env env, napi_value jsValue) {
    napi_valuetype type;
    napi_status status = napi_typeof(env, jsValue, &type);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to get type of JavaScript value");
        return nullptr;
    }

    std::string str;

    switch (type) {
        case napi_string: {
            size_t bufsize;
            status = napi_get_value_string_utf8(env, jsValue, nullptr, 0, &bufsize);
            if (status != napi_ok) {
                napi_throw_error(env, nullptr, "Failed to get the size of the string");
                return nullptr;
            }

            char* cString = new char[bufsize + 1];
            status = napi_get_value_string_utf8(env, jsValue, cString, bufsize + 1, nullptr);
            if (status != napi_ok) {
                delete[] cString;
                napi_throw_error(env, nullptr, "Failed to get the string value");
                return nullptr;
            }
            
            return cString;

        }
        case napi_object: {
            bool isArray;
            status = napi_is_array(env, jsValue, &isArray);
            if (status != napi_ok || !isArray) {
                napi_throw_error(env, nullptr, "Expected an array");
                return nullptr;
            }

            uint32_t length;
            status = napi_get_array_length(env, jsValue, &length);
            if (status != napi_ok) {
                napi_throw_error(env, nullptr, "Failed to get array length");
                return nullptr;
            }

            std::ostringstream oss;
            oss << "[";

            bool hasStringElements = false;
            bool firstElement = true;


            for (uint32_t i = 0; i < length; ++i) {
                napi_value element;
                status = napi_get_element(env, jsValue, i, &element);
                if (status != napi_ok) {
                    napi_throw_error(env, nullptr, "Failed to get array element");
                    return nullptr;
                }

                napi_valuetype elemType;
                status = napi_typeof(env, element, &elemType);
                if (status != napi_ok) {
                    napi_throw_error(env, nullptr, "Failed to get element");
                    return nullptr;
                }

                if (elemType == napi_string) {
                    hasStringElements = true;

                    size_t elemSize;

                    status = napi_get_value_string_utf8(env, element, nullptr, 0, &elemSize);
                    if (status != napi_ok) {
                        napi_throw_error(env, nullptr, "failed to get size of string element");
                        return nullptr;
                    }
                    
                    char* elemStr = new char[elemSize + 1];
                    status = napi_get_value_string_utf8(env, element, elemStr, elemSize + 1, nullptr);
                    if (status != napi_ok) {
                        delete[] elemStr; 
                        napi_throw_error(env, nullptr, "failed to get the string element value");
                        return nullptr;
                    }

                    if (!firstElement) {
                        oss << ", ";
                    }
                    oss << elemStr;
                    firstElement = false;

                    delete[] elemStr;
                }

            }

            if (!hasStringElements) {
                napi_throw_error(env, nullptr, "Array contains no string elements");
                return nullptr;
            }

            oss << "]";

            str = oss.str();
            break;
        }
        default:
            napi_throw_error(env, nullptr, "Unsupported value type");
            return nullptr;
    }

    char* cString = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), cString);
    cString[str.size()] = '\0';

    return static_cast<const char*>(cString);
}

std::vector<std::string> convert_to_string_array(const char* str) {
    std::vector<std::string> result;

    if (!str) {
        return result;
    }

    std::string input_str(str);

    if (input_str.front() == '[' && input_str.back() == ']') {
        input_str.erase(input_str.begin());
        input_str.pop_back();
    }

    std::stringstream ss(input_str);
    std::string item;

    while (std::getline(ss, item, ']')) {
        item.erase(std::remove(item.begin(), item.end(), '['), item.end());
        item.erase(std::remove(item.begin(), item.end(), ','), item.end());
        item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        item.erase(std::find_if(item.rbegin(), item.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), item.end());

        if (!item.empty()) {
            result.push_back(item);
        }
    }

    return result;
}

napi_value CreateString(napi_env env, const std::string& str) {
    napi_value result;
    napi_status status = napi_create_string_utf8(env, str.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create string");
        return nullptr;
    }
    return result;
}

Color parse_json_rgb(const std::string& jsonString) {
    Color color = {0,0,0};

    size_t rPos = jsonString.find("\"r\":");
    size_t gPos = jsonString.find("\"g\":");
    size_t bPos = jsonString.find("\"b\":");

    if (rPos != std::string::npos) {
        size_t startPos = rPos + 4;
        size_t endPos = jsonString.find_first_of(",}", startPos);
        color.r = std::stoi(jsonString.substr(startPos, endPos - startPos));
    }

    if (gPos != std::string::npos) {
        size_t startPos = gPos + 4;
        size_t endPos = jsonString.find_first_of(",}", startPos);
        color.g = std::stoi(jsonString.substr(startPos, endPos - startPos));
    }

    if (bPos != std::string::npos) {
        size_t startPos = bPos + 4;
        size_t endPos = jsonString.find_first_of(",}", startPos);
        color.b = std::stoi(jsonString.substr(startPos, endPos - startPos));
    }

    return color;
}

std::string ConvertToString(const char* charValue) {
    return std::string(charValue);
}