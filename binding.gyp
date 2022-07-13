{
  "targets": [
    {
      "target_name": "shim",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [
        "-std=c++2a",
        "-fno-exceptions"
      ],
      "sources": [ "shim.cpp",
        "util/spidev.cpp",
        "display/display.cpp",
        "display/llio/llio.cpp",
        "display/llio/rpi2in13.cpp",

      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "util/",
        "display/",
        "display/llio/",
        "vendor/bitmask/",
      ],
      "libraries": [
        "-lm",
        "-lpthread",
        "-lgpiod"
      ],
      'defines': [
        'NAPI_DISABLE_CPP_EXCEPTIONS',
        'USE_DEV_LIB'
      ],
    }
  ]
}
