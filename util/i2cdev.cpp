#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <stdexcept>

#include "i2cdev.hpp"


namespace Util {

I2C::I2C(const char* devicePath) {
  i2c =  open(devicePath, O_RDWR);
  if(i2c < 0) {
    throw std::runtime_error("Cant open i2c device");
  }
}

I2C::~I2C() {
  close(i2c);
}

void I2C::read(uint16_t addr, uint8_t* reg, size_t regLen, uint8_t* buf, size_t bufLen) {
  i2c_msg message[2] = {
      {
        .addr = addr,
        .len = regLen,
        .buf = reg,
      },
      {
        .addr = addr,
        .flags = I2C_M_RD,
        .len = bufLen,
        .buf = buf,
      }
  };
  i2c_rdwr_ioctl_data data = {
    .msgs = message,
    .nmsgs = 2,
  };

  auto ret = ioctl(i2c, I2C_RDWR, &data);
  if(ret < 0) {
    throw std::runtime_error("Error writing to i2c device");
  }
}

void I2C::write(uint16_t addr, uint8_t* buf, size_t len) {
  i2c_msg message = {
      .addr = addr,
      .len = len,
      .buf = buf,
  };
  i2c_rdwr_ioctl_data data = {
    .msgs = &message,
    .nmsgs = 1,
  };

  auto ret = ioctl(i2c, I2C_RDWR, &data);
  if(ret < 0) {
    throw std::runtime_error("Error writing to i2c device");
  }
}

}
