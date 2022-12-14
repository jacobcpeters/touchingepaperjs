#ifndef TOUCH_HPP
#define TOUCH_HPP

#include <cstdint>
#include <vector>

#include <util.hpp>
#include "touch_device.hpp"
#include "touch_structs.hpp"

namespace Touch {

class TouchDevice;

struct TouchData
{
  uint16_t x;
  uint16_t y;
  uint16_t size;
  uint16_t id;
};
typedef struct TouchData TouchData;

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

  void tick();

  std::vector<TouchEvent>* getTouchEvents();
  void clearTouchEvents();
private:
  Touch() {}

  std::vector<TouchData> touchPoints;
  std::vector<TouchEvent> touchEvents;
  // llif = low level interface
  Util::Scope<TouchDevice> llif;
};

}

#endif /* TOUCH_HPP */
