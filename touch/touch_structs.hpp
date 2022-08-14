#ifndef TOUCH_STRUCTS_HPP
#define TOUCH_STRUCTS_HPP

#include <cstdint>

namespace Touch {

enum class EventType {
  TouchStart,
  TouchMove,
  TouchEnd
};

struct TouchEvent
{
  EventType type;

  uint8_t id;

  uint16_t x;
  uint16_t y;
  uint16_t size;

  uint16_t deltaX;
  uint16_t deltaY;
};


}

#endif /* TOUCH_STRUCTS_HPP */
