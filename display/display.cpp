#include "display.hpp"
#include <cstring>

namespace EPD {

void Display::init(DisplayConfig cfg) {
  // Create a board interface
  llif = DisplayDevice::Create(cfg);

  // Create display buffer
  displayBuffer = Ref<uint8_t[]>( new uint8_t[llif->getWidth() * llif->getHeight()] );
  memset(displayBuffer.get(), cfg.clearColor, llif->getWidth() * llif->getHeight());

  // Initialize display driver
  llif->init();

  llif->clear();
}

bool Display::writeBuffer(void* buf, size_t size) {
  if(size < (llif->getWidth() * llif->getHeight())) {
    return false;
  }

  memcpy(displayBuffer.get(), buf, llif->getWidth() * llif->getHeight());
  return true;
}

void Display::draw() {
  llif->drawBuffer(displayBuffer);
}

void Display::setDisplayMode(DisplayMode mode) {
  llif->setDisplayMode(mode);
}

void Display::clear() {
  llif->clear();
}

void Display::reset() {
  llif->reset();
}

void Display::sleep() {
  llif->sleep();
}

void Display::wake() {
  llif->reset();
}


}
