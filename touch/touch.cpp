
#include "touch.hpp"

using namespace Util;

namespace Touch {

void Touch::init(const char* device_type) {
  llif = TouchDevice::Create(device_type);

  llif->init();

  maxTouchCount = llif->getMaxTouches();

  touchPoints = CreatScope<TouchData[]>( new TouchData[maxTouchCount] );
}

TouchData* Touch::getTouchData() {
  return touchPoints.get();
}

int Touch::activeTouches() {
  return llif->getActiveTouches();
}

}
