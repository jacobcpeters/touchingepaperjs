#ifndef DISPLAY_CONFIG_HPP
#define DISPLAY_CONFIG_HPP

#include <type_traits>
#include <cstring>

#include <bitmask.hpp>
#include <util.hpp>

namespace EPD {
  enum class DisplayMirror
  {
    NoMirror  = 0x00,
    XMirror   = 0x01,
    YMirror   = 0x02
  };

  enum class DisplayMode
  {
    Full, Partial
  };

  struct DisplayConfig
  {
    Util::Ref<const char[]> type;
    uint32_t clearColor;
    DisplayMirror mirror;
    DisplayMode mode;

    //  Additional generic config data can be added to this struct
    DisplayConfig(const char* type = "rpi_2in13",
                    uint32_t clearColor = 0,
                    DisplayMirror mirror = DisplayMirror::NoMirror,
                    DisplayMode mode = DisplayMode::Full)
      : clearColor(clearColor), mirror(mirror), mode(mode)
      {

        char* tmp = new char[strlen(type) + 1];
        strcpy(tmp, type);
        this->type.reset(tmp);
      }
  };

}

template<>
  struct enable_bitmask_operators<EPD::DisplayMirror>{
    static const bool enable=true;
  };

#endif /* DISPLAY_CONFIG_HPP */
