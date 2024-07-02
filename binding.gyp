{
  "targets": [
    {
      "target_name": "primary-color",
      "sources": [ "src/primary-color.cpp", "src/convert.cpp", "src/validate.cpp" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "conditions": [
        ["OS=='linux'", {
          "libraries": [ "<(module_root_dir)/lib/libprimary_image_color.so" ]
        }],
        ["OS=='win'", {
          "libraries": [ "<(module_root_dir)/primary-color/target/release/primary_image_color.dll.lib" ],
          "postbuilds": [
            {
              "postbuild": [
                "if not exist <(module_root_dir)\\build\\Release mkdir <(module_root_dir)\\build\\Release",
                "copy <(module_root_dir)\\primary-color\\target\\release\\primary_image_color.dll <(module_root_dir)\\build\\Release\\"
              ]
            }
          ]
        }]
      ]
    }
  ]
}