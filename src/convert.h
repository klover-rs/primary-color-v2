#ifndef CONVERT_H
#define CONVERT_H

#include <node_api.h>

const char* jsStringToCString(napi_env env, napi_value jsString);

#endif // CONVERT_H