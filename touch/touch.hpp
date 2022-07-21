#ifndef TOUCH_HPP
#define TOUCH_HPP

#include <cstdint>

#include <util.hpp>
#include "touch_device.hpp"

namespace Touch {

struct TouchData
{
  uint16_t x;
  uint16_t y;
  uint16_t size;
};

class Touch
{
public:
  //Basic Singleton
  static Touch& getInstance(){
    static Touch instance;

    return instance;
  }

  Touch(Touch const&)         = delete;
  void operator=(Touch const&)  = delete;

  void init(const char* device_type);

  // returns a pointer to the touchPoints array
  TouchData* getTouchData();
  int activeTouches();
private:
  Touch() {}
  uint8_t maxTouchCount;

  Scope<TouchData[]> touchPoints;
  // llif = low level interface
  Scope<TouchDevice> llif;
};

}

#endif /* TOUCH_HPP */
