#ifndef LLIO_HPP
#define LLIO_HPP

#include <cstdint>

#include <util.hpp>

#include "../config.hpp"

namespace EPD {

  class LLIO {
  public:
    LLIO() {}
    virtual ~LLIO() {}

    virtual void init() = 0;
    //virtual void mirror(bool, bool) = 0;

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    virtual void reset() = 0;
    virtual void clear() = 0;
    virtual void sleep() = 0;


    virtual void setDisplayMode(DisplayMode) = 0;
    virtual void drawBuffer(Ref<uint8_t[]> buf, bool fullRefresh = false) = 0;

    static Scope<LLIO> Create(DisplayConfig& cfg);


  protected:
    DisplayConfig cfg;
  };

};


#endif /* LLIO_HPP */