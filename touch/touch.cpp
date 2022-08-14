
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

TouchData* Touch::getTouchEvents() {
  return touchPoints.data();
}

void Touch::tick() {
  int activeTouches = llif->getActiveTouches();
  if(activeTouches < 1) {
    // create TouchEndEvents if there are no more active touch points
    for(auto touch = touchPoints.b bn.mnegin(); touch != touchPoints.end(); touch++) {
      // stub: create TouchEndEvent
      touchPoints.erase(touch);
    }
    return;
  }

  Scope<TouchData[]> touchBuffer = CreatScope<TouchData[]>(activeTouches);
  llif->getTouchData(touchBuffer.get(), activeTouches);

  for(auto touch = touchPoints.begin(); touch != touchPoints.end(); touch++) {
    TouchData* data = fetchTouchId(touch->id, touchBuffer.get(), activeTouches);
    if(data != nullptr) {
      //
      //stub: create TouchMoveEvent
      *touch = *data;
      // set id to a recognizable number to be sorted out;
      data->id = 0xDEAD;
    } else {
      //stub: create TouchEndEvent
      touchPoints.erase(touch);
    }
  }
  for(auto i = 0; i < activeTouches; i++) {
    if(touchBuffer[i].id != 0xDEAD) {
      //stub: create TouchStartEvent
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
