#ifndef CONVERT_H
#define CONVERT_H

#include <node_api.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cctype>  

const char* jsStringToCString(napi_env env, napi_value jsValue);

std::vector<std::string> convert_to_string_array(const char* str);

#endif // CONVERT_H