
#include "touch.hpp"

#include <limits>

using namespace Util;



namespace Touch {
int containsId(uint16_t id, TouchData data, size_t touches);

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
    return;
  }

  Scope<TouchData[]> touchBuffer = CreatScope<TouchData[]>( new TouchData[activeTouches]);
  llif->getTouchData(touchBuffer.get(), activeTouches);

  for(auto touch = touchPoints.begin(); touch != touchPoints.end(); touch++) {
    int index = containsId(touch->id, touchBuffer.get(), activeTouches);
    if(index) {
      //stub: create TouchMoveEvent
      *touch = touchBuffer[index];
      //set id to a recognizable number to be sorted out;
      touchBuffer[index].id = 0xDEAD;
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

int containsId(uint16_t id, TouchData* data, size_t touches) {
  for(auto i = 0; i < touches; i++) {
    if(data[i].id == id)
      return i;
  }
  return 0;
}

}
