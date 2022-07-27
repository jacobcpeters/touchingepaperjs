
#include "rpi_gt1151.hpp"
#include <unistd.h>

namespace Touch {

static struct {
  uint8_t status[2] = {0x81, 0x4E};
  uint8_t touch[2] = {0x81, 0x4F};
} reg;

struct StatusBuffer {
  uint8_t status : 1;
  uint8_t large : 1;
  uint8_t proximity : 1;
  uint8_t haveKey : 1;
  uint8_t touchPoints : 3;
};
typedef struct StatusBuffer StatusBuffer;

struct TouchBuffer {
  uint8_t id;

  struct {
    uint8_t low;
    uint8_t high;
  } x;

  struct {
    uint8_t low;
    uint8_t high;
  } y;

  struct {
    uint8_t low;
    uint8_t high;
  } s;

  uint8_t _pad;
};
typedef struct TouchBuffer TouchBuffer;


RpiGt1151::RpiGt1151(): i2c("/dev/i2c-1") {
  //GPIO Init
  gpio = gpiod_chip_open_by_name("gpiochip0");
  resetPin = gpiod_chip_get_line(gpio, 22);

  gpiod_line_request_output(resetPin, "touch_driver", 1);

  reset();
}

RpiGt1151::~RpiGt1151() {
  gpiod_line_release(resetPin);
  gpiod_chip_close(gpio);
}

void RpiGt1151::init() {
  //unused at this time
}

void RpiGt1151::reset() {
  gpiod_line_set_value(resetPin, 0);
  usleep(200);
  gpiod_line_set_value(resetPin, 1);
}

int RpiGt1151::getWidth() {
  return 122;
}

int RpiGt1151::getHeight() {
  return 250;
}

void RpiGt1151::getTouchData(TouchData* data, size_t len) {
  TouchBuffer buf[len];

  i2c.read(0x14, reg.touch, 2, reinterpret_cast<uint8_t*>(buf), sizeof(TouchBuffer) * len);
  for(auto i = 0; i < len; i++) {
    data[i].id = buf[i].id;
    endian_swap(data[i].x, buf[i].x.high, buf[i].x.low);
    endian_swap(data[i].y, buf[i].y.high, buf[i].y.low);
    endian_swap(data[i].size, buf[i].s.high, buf[i].s.low);
  }

  //clear status register
  uint8_t clear[3] = {reg.status[0], reg.status[1], 0};
  i2c.write(0x14, clear, 3);
}

int RpiGt1151::maxTouches() {
  return 5;
}

int RpiGt1151::getActiveTouches() {
  StatusBuffer status;
  i2c.read(0x14, reg.status, 2, reinterpret_cast<uint8_t*>(&status), 1);

  if(!status.status || !status.haveKey)
    return 0;

  return status.touchPoints;
}

inline void endian_swap(uint16_t&val, uint8_t high, uint8_t low) {
  val = high;
  val <<= 8;
  val += low;
}

}
