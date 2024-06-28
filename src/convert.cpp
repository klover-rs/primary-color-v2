#include <node_api.h>

const char* jsStringToCString(napi_env env, napi_value jsString) {
    size_t bufsize;

    napi_get_value_string_utf8(env, jsString, nullptr, 0, &bufsize);

    char* cString = new char[bufsize + 1];
    napi_get_value_string_utf8(env, jsString, cString, bufsize + 1, nullptr);

    return static_cast<const char*>(cString);
}