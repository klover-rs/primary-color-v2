{
  "targets": [
    {
      "target_name": "primary-color",
      "sources": [ "src/primary-color.cpp", "src/convert.cpp", "src/validate.cpp" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "conditions": [
        ["OS=='linux'", {
          "libraries": [ "<(module_root_dir)/lib/libprimary_image_color.a" ]
        }],
        ["OS=='win'", {
          "libraries": [ "<(module_root_dir)\\primary-color\\target\\release\\primary_image_color.dll.lib" ],
        }],
        ["OS=='mac'", {
          "libraries": [ "<(module_root_dir)/lib/libprimary_image_color.a" ]
        }]
      ]
    }
  ]
}