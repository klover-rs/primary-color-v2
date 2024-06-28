#include <node_api.h>
#include "header.h"
#include "convert.h"

namespace demo {


napi_value PrimaryColor(napi_env env, napi_callback_info info) {

  size_t argc = 1;

  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  if (argc < 1) {
    napi_throw_error(env, nullptr, "Missing image URL argument");
    return nullptr;
  }

  const char* image_url = jsStringToCString(env, argv[0]);

  const char* hex_code = primary_color_from_image_url(image_url);

  delete[] const_cast<char*>(image_url);

  if (hex_code == nullptr) {
    napi_throw_error(env, nullptr, "Oh uhm :/ it seems like we encountered an null pointer, please check the console for more details.");
    return nullptr;
  }

  napi_value result;
  napi_status status;

  status = napi_create_string_utf8(env, hex_code, NAPI_AUTO_LENGTH, &result);
  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "Failed to create result string");
    return nullptr;
  }

  delete[] const_cast<char*>(hex_code);

  return result;
}

napi_value Method(napi_env env, napi_callback_info args) {
  napi_value greeting;
  napi_status status;

  status = napi_create_string_utf8(env, "hello world", NAPI_AUTO_LENGTH, &greeting);
  if (status != napi_ok) return nullptr;
  return greeting;
}

napi_value init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, nullptr, 0, Method, nullptr, &fn);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "hello", fn);
  if (status != napi_ok) return nullptr;

  status = napi_create_function(env, nullptr, 0, PrimaryColor, nullptr, &fn);
  if (status != napi_ok) return nullptr;
  status = napi_set_named_property(env, exports, "primaryColor", fn);
  if (status != napi_ok) return nullptr;

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}  