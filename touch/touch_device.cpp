#include "touch_device.hpp"

#include <cstring>
#include <stdexcept>

#include "rpi_gt1151.hpp"

using namespace Util;

namespace Touch {

Scope<TouchDevice> TouchDevice::Create(const char* device_type) {
  if(strcmp(device_type, "rpi_2in13") == 0) {
    return CreatScope<RpiGt1151>();
  } else {
    throw std::runtime_error("invalid touch type");
  }
}

}
