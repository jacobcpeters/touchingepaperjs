#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>

#include <stdexcept>
#include <cstring>

#include "spidev.hpp"

namespace Util {

SPI::SPI(const char* devicePath) :
  spi(0), mode(SPIMode::Mode_0), bits(0), speed(0), delay(0) {
  spi = open(devicePath, O_RDWR);
  if(spi < 0) {
    throw std::runtime_error("Cant open SPI device");
  }
}

SPI::~SPI() {
  close(spi);
}

bool SPI::setMode(SPIMode mode) {
  this->mode = mode;
  uint32_t modeBuf = static_cast<uint32_t>(mode);
  if(ioctl(spi, SPI_IOC_WR_MODE32, &modeBuf) == -1) {
    return false;
  }

  return true;
}

bool SPI::setBitsPerWord(uint8_t bits) {
  this->bits = bits;
  if(ioctl(spi, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
    return false;
  } else {
    return true;
  }
}

bool SPI::setSpeed(uint32_t speed) {
  this->speed = speed;
  if(ioctl(spi, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
    return false;
  } else {
    return true;
  }
}

bool SPI::setDelay(uint16_t delay) {
  this->delay = delay;
  return true;
}

bool SPI::write(const uint8_t* tx, size_t len) {
  struct spi_ioc_transfer xfer[1];
  memset(xfer, 0, sizeof(xfer));

  xfer[0].delay_usecs = delay;
  xfer[0].tx_buf = reinterpret_cast<uint64_t>(tx);

  if(!(mode == SPIMode::Loop) && mode == (SPIMode::TxQuad | SPIMode::TxDual)) {
    xfer[0].rx_buf = 0;
  }

  if(ioctl(spi, SPI_IOC_MESSAGE(1), xfer) < 1) {
    return false;
  }

  return true;
}

bool SPI::read(const uint8_t* rx, size_t len) {
  struct spi_ioc_transfer xfer[1];
  memset(xfer, 0, sizeof(xfer));

  xfer[0].delay_usecs = delay;
  xfer[0].rx_buf = reinterpret_cast<uint64_t>(rx);

  if(!(mode == SPIMode::Loop) && mode == (SPIMode::RxQuad | SPIMode::RxDual)) {
    xfer[0].tx_buf = 0;
  }

  if(ioctl(spi, SPI_IOC_MESSAGE(1), xfer) < 1) {
    return false;
  }

  return true;
}

bool SPI::transfer(const uint8_t* tx, const uint8_t* rx, size_t len) {
  struct spi_ioc_transfer xfer[1];
  memset(xfer, 0, sizeof(xfer));

  xfer[0].delay_usecs = delay;
  xfer[0].tx_buf = reinterpret_cast<uint64_t>(tx);
  xfer[0].rx_buf = reinterpret_cast<uint64_t>(rx);

  if(!(mode == SPIMode::Loop)) {
    if(mode == (SPIMode::TxQuad | SPIMode::TxDual)) {
      xfer[0].rx_buf = 0;
    } else if (mode == (SPIMode::RxQuad | SPIMode::RxDual)) {
      xfer[0].tx_buf = 0;
    }
  }

  if(ioctl(spi, SPI_IOC_MESSAGE(1), xfer) < 1) {
    return false;
  }

  return true;
}

}
