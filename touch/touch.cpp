
#include "touch.hpp"

#include <limits>
#include <iostream>

using namespace Util;



namespace Touch {
TouchData* fetchTouchId(uint16_t id, TouchData* data, size_t touches);

void Touch::init(const char* device_type) {
  llif = TouchDevice::Create(device_type);

  llif->init();

  touchPoints.reserve( llif->maxTouches() );
}

TouchEvent* Touch::getTouchEvents() {
  return touchEvents.data();
}

void Touch::clearTouchEvents() {
  touchEvents.clear();
}

void Touch::tick() {
  TouchEvent ev = {};

  int activeTouches = llif->getActiveTouches();
  if(activeTouches < 1) {
    // create TouchEndEvents if there are no more active touch points
    for(auto touch = touchPoints.begin(); touch != touchPoints.end(); touch++) {
      ev = {
        .type = EventType::TouchEnd,
        .id = touch->id,

        .x = touch->x,
        .y = touch->y,
        .size = touch->size,

        .deltaX = 0,
        .deltaY = 0
      };
      touchEvents.push_back(ev);

      touchPoints.erase(touch);
    }
    return;
  }

  Scope<TouchData[]> touchBuffer = CreatScope<TouchData[]>(activeTouches);
  llif->getTouchData(touchBuffer.get(), activeTouches);

  for(auto touch = touchPoints.begin(); touch != touchPoints.end(); touch++) {
    TouchData* data = fetchTouchId(touch->id, touchBuffer.get(), activeTouches);
    if(data != nullptr) {
      ev = {
        .type = EventType::TouchMove,
        .id = touch->id,

        .x = touch->x,
        .y = touch->y,
        .size = touch->size,

        .deltaX = touch->x - data->x,
        .deltaY = touch->y - data->y
      };
      touchEvents.push_back(ev);

      *touch = *data;
      // set id to a recognizable number to be sorted out;
      data->id = 0xDEAD;
    } else {
      ev = {
        .type = EventType::TouchEnd,
        .id = touch->id,

        .x = touch->x,
        .y = touch->y,
        .size = touch->size,

        .deltaX = 0,
        .deltaY = 0
      };
      touchEvents.push_back(ev);

      touchPoints.erase(touch);
    }
  }
  for(auto i = 0; i < activeTouches; i++) {
    if(touchBuffer[i].id != 0xDEAD) {
      ev = {
        .type = EventType::TouchStart,
        .id = touchBuffer[i].id,

        .x = touchBuffer[i].x,
        .y = touchBuffer[i].y,
        .size = touchBuffer[i].size,

        .deltaX = 0,
        .deltaY = 0
      };
      touchEvents.push_back(ev);

      touchPoints.push_back(touchBuffer[i]);
    }
  }
}

TouchData* fetchTouchId(uint16_t id, TouchData* data, size_t touches) {
  for(size_t i = 0; i < touches; i++) {
    if(data[i].id == id)
      return &data[i];
  }
  return nullptr;
}

}
