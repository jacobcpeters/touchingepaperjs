#ifndef I2CDEV_HPP
#define I2CDEV_HPP

#include <cstdint>

namespace Util {

class I2C
{
public:
  I2C(const char* devicePath);
  ~I2C();

  void read(uint16_t addr, uint8_t* reg, size_t regLen, uint8_t* buf, size_t bufLen);
  void write(uint16_t addr, uint8_t* buf, size_t len);

private:
  int i2c;
};

}

#endif /* I2CDEV_HPP */
