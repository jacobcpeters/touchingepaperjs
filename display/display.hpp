#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <cstdint>

#include "config.hpp"
#include "llio/llio.hpp"

namespace EPD {

  class Display {
    public:
      //Basic Singleton
      static Display& getInstance(){
        static Display instance;

        return instance;
      }
    private:
      Display() {}
    public:
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
      Ref<uint8_t[]> displayBuffer;
      Scope<LLIO> llio;
  };
};


#endif /* DISPLAY_HPP */
