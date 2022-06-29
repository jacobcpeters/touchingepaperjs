{
  "targets": [
    {
      "target_name": "shim",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "shim.cc",
        "vendor/epaper/c/lib/EPD/EPD_2in13_V3.c",
        "vendor/epaper/c/lib/Driver/GT1151.c",
        "vendor/epaper/c/lib/Config/DEV_Config.c",
        "vendor/epaper/c/lib/Config/RPI_sysfs_gpio.c",
        "vendor/epaper/c/lib/Config/dev_hardware_SPI.c",
        "vendor/epaper/c/lib/Config/dev_hardware_i2c.c",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "vendor/epaper/c/lib/EPD/",
        "vendor/epaper/c/lib/Driver/",
        "vendor/epaper/c/lib/Config/",
      ],
      "libraries": [
        "-lm",
        "-lpthread"
      ],
      'defines': [
        'NAPI_DISABLE_CPP_EXCEPTIONS',
        'USE_DEV_LIB'
      ],
    }
  ]
}
