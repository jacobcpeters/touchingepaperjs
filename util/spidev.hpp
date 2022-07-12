#ifndef SPIDEV_HPP
#define SPIDEV_HPP

#include <cstdint>
#include <bitmask.hpp>

namespace Util {

enum class SPIMode : uint32_t {
  CPHA      = 0x01,
  CPOL      = 0x02,
  Mode_0    = 0x00,
  Mode_1    = CPHA,
  Mode_2    = CPOL,
  Mode_3    = 0x03,
  CsHigh	  = 0x04,
  LsbFirst  = 0x08,
  ThreeWire = 0x10,
  Loop      = 0x20,
  NoCs      = 0x40,
  Ready     = 0x80,
  TxDual    = 0x100,
  TxQuad    = 0x200,
  RxDual    = 0x400,
  RxQuad    = 0x800
};

class SPI
{
public:
  SPI(const char* devicePath);
  ~SPI();

  bool setMode(SPIMode);
  bool setBitsPerWord(uint8_t);
  bool setSpeed(uint32_t);
  bool setDelay(uint16_t);

  bool write(uint8_t const*, size_t);
  bool read(uint8_t const*, size_t);
  bool transfer(uint8_t const*, uint8_t const*, size_t);

private:
  int32_t spi;
  SPIMode mode;
  uint8_t bits;
  uint32_t speed;
  uint16_t delay;
};

}

template<>
  struct enable_bitmask_operators<Util::SPIMode>{
    static const bool enable=true;
  };

#endif /* SPIDEV_HPP */
