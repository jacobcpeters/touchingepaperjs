{
  "targets": [
    {
      "target_name": "shim",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [
        "-std=c++2a",
        "-fno-exceptions"
      ],
      'conditions': [
        ['OS=="linux"', {
          "sources": [ "shim.cpp",
            "util/spidev.cpp",
            "util/i2cdev.cpp",
            "display/display.cpp",
            "display/display_device.cpp",
            "display/device/rpi2in13.cpp",
            "touch/touch.cpp",
            "touch/touch_device.cpp",
            "touch/device/rpi_gt1151.cpp",
          ],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")",
            "util/",
            "display/",
            "display/device/",
            "touch/",
            "touch/device/",
            "vendor/bitmask/",
          ],
          "libraries": [
            "-lm",
            "-lpthread",
            "-lgpiod"
          ],
          'defines': [
            'NAPI_DISABLE_CPP_EXCEPTIONS',
          ],
        }],
        ['OS!="linux"', {"sources": ["shim_mock.cpp"]}]
      ],
    }
  ]
}
