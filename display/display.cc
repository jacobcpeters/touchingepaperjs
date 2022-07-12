#include "display.hpp"
#include <cstring>

namespace EPD {

void Display::init(DisplayConfig cfg) {
  // Create a board interface
  llio = LLIO::Create(cfg);

  // Create display buffer
  displayBuffer = Ref<uint8_t[]>( new uint8_t[llio->getWidth() * llio->getHeight()] );
  memset(displayBuffer.get(), cfg.clearColor, llio->getWidth() * llio->getHeight());

  // Initialize display driver
  llio->init();

  llio->clear();
}

bool Display::writeBuffer(void* buf, size_t size) {
  if(size < (llio->getWidth() * llio->getHeight())) {
    return false;
  }

  memcpy(displayBuffer.get(), buf, llio->getWidth() * llio->getHeight());
  return true;
}

void Display::draw() {
  llio->drawBuffer(displayBuffer);
}

void Display::setDisplayMode(DisplayMode mode) {
  llio->setDisplayMode(mode);
}

void Display::clear() {
  llio->clear();
}

void Display::reset() {
  llio->reset();
}

void Display::sleep() {
  llio->sleep();
}

void Display::wake() {
  llio->reset();
}


}
