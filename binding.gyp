{
  "targets": [
    {
      "target_name": "primary-color",
      "sources": [ "src/primary-color.cpp", "src/convert.cpp", "src/validate.cpp" ],
      "libraries": [ "<(module_root_dir)/lib/libprimary_image_color.so" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}