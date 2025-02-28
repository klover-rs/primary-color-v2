#include <node_api.h>
#include "primary_color_lib.h"
#include "convert.h"
#include "validate.h"
#include <iostream>
#include <vector>
#include <string>

namespace demo {

napi_value PrimaryColorByImageUrl(napi_env env, napi_callback_info info) {
  size_t argc = 2;

  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  if (argc < 2) {
    napi_throw_error(env, nullptr, "Missing arguments: expected image URL and color format");
    return nullptr;
  }

  const char* image_url = jsStringToCString(env, argv[0]);
  const char* format_type = jsStringToCString(env, argv[1]);

  HexOrRgb format;

  if (strcmp(format_type, "hex") == 0) {
      format = HexOrRgb::Hex;
  }
  else if (strcmp(format_type, "rgb") == 0) {
      format = HexOrRgb::Rgb;
  }
  else {
      delete[] const_cast<char*>(image_url);
      delete[] const_cast<char*>(format_type);
      napi_throw_error(env, nullptr, "Invalid format type. Use 'hex' or 'rgb'.");
      return nullptr;
  }

  const char* hex_code = primary_color_from_image_url(image_url, format);

  delete[] const_cast<char*>(image_url);
  delete[] const_cast<char*>(format_type);

  if (hex_code == nullptr) {
    napi_throw_error(env, nullptr, "Oh uhm :/ it seems like we encountered an null pointer, please check the console for more details.");
    return nullptr;
  }

  napi_value result;
  napi_status status;

  bool is_array = check_if_is_array(hex_code);

  if (is_array) {
    std::vector<std::string> colors = convert_to_string_array(hex_code);

    napi_value js_array;
    status = napi_create_array(env, &js_array);
    if (status != napi_ok) {
      napi_throw_error(env, nullptr, "failed to create javascript array");
      delete[] const_cast<char*>(hex_code);
      return nullptr;
    }

    if (format == HexOrRgb::Rgb) {
      for (size_t i = 0; i < colors.size(); ++i) {
        napi_value jsObject;

        Color color = parse_json_rgb(colors[i]);

        status = napi_create_object(env, &jsObject);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to create object");
          
          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        napi_value r, g, b;

        status = napi_create_int32(env, color.r, &r);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "Failed to create property r");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_create_int32(env, color.g, &g);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to create property g");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_create_int32(env, color.b, &b);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to create property b");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_set_named_property(env, jsObject, "r", r);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set property r");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_set_named_property(env, jsObject, "g", g);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "Failed to set property g");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_set_named_property(env, jsObject, "b", b);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set property b");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }


        status = napi_set_element(env, js_array, i, jsObject);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set array element");
        
          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }
      }

      result = js_array;
    } else {
      for (size_t i = 0; i < colors.size(); ++i) {
        napi_value js_string;
        status = napi_create_string_utf8(env, colors[i].c_str(), NAPI_AUTO_LENGTH, &js_string);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "Failed to create string for array element");
        
          delete[] const_cast<char*>(hex_code);
        }
        status = napi_set_element(env, js_array, i, js_string);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set array element");
        
          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }
      }

      result = js_array;
    }

  } else {

    if (format == HexOrRgb::Rgb) {
      napi_value jsObject;

      std::string color_str = ConvertToString(hex_code);

      Color color = parse_json_rgb(color_str);

      status = napi_create_object(env, &jsObject);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to create object");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      napi_value r, g, b;

      status = napi_create_int32(env, color.r, &r);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create property r");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_create_int32(env, color.g, &g);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to create property g");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_create_int32(env, color.b, &b);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to create property b");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_set_named_property(env, jsObject, "r", r);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to set property r");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_set_named_property(env, jsObject, "g", g);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to set property g");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_set_named_property(env, jsObject, "b", b);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to set property b");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }
      result = jsObject;

    } else {
      status = napi_create_string_utf8(env, hex_code, NAPI_AUTO_LENGTH, &result);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create result string");
        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }
    }
  }

  delete[] const_cast<char*>(hex_code);

  return result;
}

napi_value PrimaryColorByBase64(napi_env env, napi_callback_info info) {

  size_t argc = 2;

  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  if (argc < 1) {
    napi_throw_error(env, nullptr, "Missing image URL argument");
    return nullptr;
  }

  const char* base64 = jsStringToCString(env, argv[0]);
  const char* format_type = jsStringToCString(env, argv[1]);

  HexOrRgb format;

  if (strcmp(format_type, "hex") == 0) {
    format = HexOrRgb::Hex;
  }
  else if (strcmp(format_type, "rgb") == 0) {
    format = HexOrRgb::Rgb;
  }
  else {
    delete[] const_cast<char*>(base64);
    delete[] const_cast<char*>(format_type);
    napi_throw_error(env, nullptr, "Invalid format type. Use 'hex' or 'rgb'.");
    return nullptr;
  }

  const char* hex_code = primary_color_from_base64(base64, format);

  delete[] const_cast<char*>(base64);
  delete[] const_cast<char*>(format_type);

  if (hex_code == nullptr) {
    napi_throw_error(env, nullptr, "Oh uhm :/ it seems like we encountered an null pointer, please check the console for more details.");
    return nullptr;
  } 

  napi_value result;
  napi_status status;

  bool is_array = check_if_is_array(hex_code);

  if (is_array) {
    std::vector<std::string> colors = convert_to_string_array(hex_code);

    napi_value js_array;
    status = napi_create_array(env, &js_array);
    if (status != napi_ok) {
      napi_throw_error(env, nullptr, "failed to create javascript array");
      delete[] const_cast<char*>(hex_code);
      return nullptr; 
    }

    if (format == HexOrRgb::Rgb) {
      for (size_t i = 0; i < colors.size(); ++i) {
      
        napi_value jsObject;

        Color color = parse_json_rgb(colors[i]);
    

        status = napi_create_object(env, &jsObject); 
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to create object");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        napi_value r, g, b;

        status = napi_create_int32(env, color.r, &r);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "Failed to create property r");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_create_int32(env, color.g, &g);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to create property g");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_create_int32(env, color.b, &b);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to create property b");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_set_named_property(env, jsObject, "r", r);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set property r");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_set_named_property(env, jsObject, "g", g);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "Failed to set property g");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }

        status = napi_set_named_property(env, jsObject, "b", b);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set property b");

          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }


        status = napi_set_element(env, js_array, i, jsObject);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set array element");
        
          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }
      }

      result = js_array;
    } else {
      for (size_t i = 0; i < colors.size(); ++i) {
        napi_value js_string;
        status = napi_create_string_utf8(env, colors[i].c_str(), NAPI_AUTO_LENGTH, &js_string);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "Failed to create string for array element");
        
          delete[] const_cast<char*>(hex_code);
        }
        status = napi_set_element(env, js_array, i, js_string);
        if (status != napi_ok) {
          napi_throw_error(env, nullptr, "failed to set array element");
        
          delete[] const_cast<char*>(hex_code);
          return nullptr;
        }
      }

      result = js_array;
    }
  } else {

    if (format == HexOrRgb::Rgb) {
      napi_value jsObject;

      std::string color_str = ConvertToString(hex_code);

      Color color = parse_json_rgb(color_str);

      status = napi_create_object(env, &jsObject);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to create object");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      napi_value r, g, b;

      status = napi_create_int32(env, color.r, &r);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create property r");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_create_int32(env, color.g, &g);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to create property g");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_create_int32(env, color.b, &b);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to create property b");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_set_named_property(env, jsObject, "r", r);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to set property r");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_set_named_property(env, jsObject, "g", g);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to set property g");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }

      status = napi_set_named_property(env, jsObject, "b", b);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "failed to set property b");

        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }
      result = jsObject;

    } else {
      status = napi_create_string_utf8(env, hex_code, NAPI_AUTO_LENGTH, &result);
      if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create result string");
        delete[] const_cast<char*>(hex_code);
        return nullptr;
      }
    }

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

  status = napi_create_function(env, nullptr, 0, PrimaryColorByImageUrl, nullptr, &fn);
  if (status != napi_ok) return nullptr;
  status = napi_set_named_property(env, exports, "primaryColorByImageUrl", fn);
  if (status != napi_ok) return nullptr;

  status = napi_create_function(env, nullptr, 0, PrimaryColorByBase64, nullptr, &fn);
  if (status != napi_ok) return nullptr;
  status = napi_set_named_property(env, exports, "primaryColorByBase64", fn);
  if (status != napi_ok) return nullptr;

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}  