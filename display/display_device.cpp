#include "display_device.hpp"

#include <cstring>
#include <stdexcept>

#include "rpi2in13.hpp"

namespace EPD {

Scope<DisplayDevice> DisplayDevice::Create(DisplayConfig& cfg) {
  if(strcmp(cfg.type.get(), "rpi_2in13") == 0) {
    return CreatScope<RPi2in13>(cfg);
  } else {
    throw std::runtime_error("invalid display type");
  }
}

}
