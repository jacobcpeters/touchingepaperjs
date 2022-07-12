{
  "targets": [
    {
      "target_name": "shim",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [
        "-std=c++2a",
        "-fno-exceptions"
      ],
      "sources": [ "shim.cc",
        "util/spidev.cc",
        "display/display.cc",
        "display/llio/llio.cc",
        "display/llio/rpi2in13.cc",

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
