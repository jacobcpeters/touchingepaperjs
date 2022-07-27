#ifndef RPI_GT1151_HPP
#define RPI_GT1151_HPP

#include <gpiod.h>

#include "touch_device.hpp"
#include <i2cdev.hpp>

namespace Touch
{

class RpiGt1151: public TouchDevice
{
public:
  RpiGt1151();
  ~RpiGt1151();

  void init();

  int getWidth();
  int getHeight();

  void getTouchData(TouchData* data, size_t len);

  int maxTouches();
  int getActiveTouches();

  void reset();
private:
  Util::I2C i2c;

  gpiod_chip* gpio;
  gpiod_line* resetPin;

};

} // namespace Touch


#endif /* RPI_GT1151_HPP */
