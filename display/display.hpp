#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <cstdint>

#include <util.hpp>
#include "display_config.hpp"
#include "display_device.hpp"

namespace EPD {

  class Display {
    public:
      //Basic Singleton
      static Display& getInstance(){
        static Display instance;

        return instance;
      }

      Display(Display const&)         = delete;
      void operator=(Display const&)  = delete;

      //Begin Actual Class definition
      void init(DisplayConfig cfg);

      void setDisplayMode(DisplayMode mode);
      void clear();
      void reset();
      void sleep();
      void wake();

      bool writeBuffer(void* buf, size_t size);
      void draw();

    private:
      Display() {}

      Util::Ref<uint8_t[]> displayBuffer;
      // llif = low level interface
      Util::Scope<DisplayDevice> llif;
  };
};


#endif /* DISPLAY_HPP */
