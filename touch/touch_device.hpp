#ifndef TOUCH_DEVICE_HPP
#define TOUCH_DEVICE_HPP

#include <cstdint>

#include <util.hpp>

#include "touch.hpp"

namespace Touch {

class TouchDevice {
public:
  TouchDevice() {}
  virtual ~TouchDevice() {}

  virtual void init() = 0;

  virtual int getWidth() = 0;
  virtual int getHeight() = 0;

  virtual void getTouchData(TouchData* data, size_t len);

  virtual int maxTouches() = 0;
  virtual int getActiveTouches() = 0;

  virtual void reset() = 0;

  static Util::Scope<TouchDevice> Create(const char* device_type);

};

};

#endif /* DISPLAY_DEVICE_HPP */
