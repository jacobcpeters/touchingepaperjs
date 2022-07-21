#ifndef TOUCH_DEVICE_HPP
#define TOUCH_DEVICE_HPP

#include <cstdint>

#include <util.hpp>

namespace Touch {

class TouchDevice {
public:
  TouchDevice() {}
  virtual ~TouchDevice() {}

  virtual void init() = 0;

  virtual int getWidth() = 0;
  virtual int getHeight() = 0;

  virtual void getTouchData(TouchData* data);

  virtual int getMaxTouches() = 0;
  virtual int getActiveTouches() = 0;

  virtual void reset() = 0;

  static Scope<TouchDevice> Create(const char* device_type);

};

};

#endif /* DISPLAY_DEVICE_HPP */
