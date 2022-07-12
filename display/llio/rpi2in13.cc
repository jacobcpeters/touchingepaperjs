#include <cstring>
#include <unistd.h>

#include "rpi2in13.hpp"

const uint8_t lut_full_update[]= {
  0x80,0x60,0x40,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
  0x10,0x60,0x20,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
  0x80,0x60,0x40,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
  0x10,0x60,0x20,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7

  0x03,0x03,0x00,0x00,0x02,                       // TP0 A~D RP0
  0x09,0x09,0x00,0x00,0x02,                       // TP1 A~D RP1
  0x03,0x03,0x00,0x00,0x02,                       // TP2 A~D RP2
  0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
  0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
  0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
  0x00,0x00,0x00,0x00,0x00,                       // TP6 A~D RP6
};

const uint8_t lut_partial_update[]= { //20 bytes
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
  0x80,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
  0x40,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7

  0x0A,0x00,0x00,0x00,0x00,                       // TP0 A~D RP0
  0x00,0x00,0x00,0x00,0x00,                       // TP1 A~D RP1
  0x00,0x00,0x00,0x00,0x00,                       // TP2 A~D RP2
  0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
  0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
  0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
  0x00,0x00,0x00,0x00,0x00,                       // TP6 A~D RP6
};

namespace EPD {

inline uint8_t GateDrivingVoltage(float voltage);
uint8_t VSHVoltage(float voltage);
uint8_t VshVoltageLow(float voltage);
uint8_t VshVoltageHigh(float voltage);
uint8_t VslVoltage(float voltage);

uint8_t dummyLineFromHz(uint8_t hz);
uint8_t gateLineFromHz(uint8_t hz);

RPi2in13::RPi2in13(DisplayConfig config = DisplayConfig())
  : spi("/dev/spidev0.0"),
    width(16), height(250)
{
  this->cfg = config;

  //initialize SPI settings
  spi.setMode(Util::SPIMode::Mode_0);
  spi.setSpeed(10000000);

  //GPIO Init
  gpio = gpiod_chip_open_by_name("gpiochip0");
  csPin     = gpiod_chip_get_line(gpio, 8);
  dcPin     = gpiod_chip_get_line(gpio, 25);
  resetPin  = gpiod_chip_get_line(gpio, 17);
  busyPin   = gpiod_chip_get_line(gpio, 24);

  gpiod_line_request_output(csPin, "epd_driver", 1);
  gpiod_line_request_output(dcPin, "epd_driver", 0);
  gpiod_line_request_output(resetPin, "epd_driver", 0);
  gpiod_line_request_input(busyPin, "epd_driver");

  gpiod_line_set_value(resetPin, 1);
}

RPi2in13::~RPi2in13() {
  gpiod_line_release(csPin);
  gpiod_line_release(dcPin);
  gpiod_line_release(resetPin);
  gpiod_line_release(busyPin);
  gpiod_chip_close(gpio);
}

void RPi2in13::init() {
  waitForBusy();
  sendCommand(Command::SoftwareReset);
  waitForBusy();

  sendCommand(Command::SetAnalogBlockControl, 0x54);
  sendCommand(Command::SetDigitalBlockControl, 0x3B);

  uint8_t docData[] = { height - 1, 0, 0 };
  sendCommand(Command::DriverOutput, docData, 3);

  uint8_t sdvcData[] = { VSHVoltage(15.0f), VSHVoltage(5.0f), VslVoltage(-15.0f) };
  sendCommand(Command::SourceDrivingVoltageControl, sdvcData, 3);
  sendCommand(Command::GateDrivingVoltageControl, GateDrivingVoltage(19.0f));


  sendCommand(Command::SetDummyLinePeriod, dummyLineFromHz(50));
  sendCommand(Command::SetGateLineWidth, gateLineFromHz(50));

  switch (cfg.mode)
    {
    case DisplayMode::Partial:
      setDisplayPartial();
      break;
    case DisplayMode::Full:
    default:
      setDisplayFull();
      break;
    }

  waitForBusy();
}

void RPi2in13::initRamSettings() {
  sendCommand(Command::DataEntryModeSetting, static_cast<uint8_t>(cfg.mirror));

  uint8_t xStartEnd[] = { 0, 0 };
  if(cfg.mirror == DisplayMirror::XMirror) {
    xStartEnd[1] = width - 1;
  } else {
    xStartEnd[0] = width - 1;
  }
  sendCommand(Command::SetRamXStartEnd, xStartEnd, 2);
  sendCommand(Command::SetRamXCounter, xStartEnd[0]);

  uint8_t yStartEnd[] = { 0, 0, 0, 0 };
  if(cfg.mirror == DisplayMirror::YMirror) {
    yStartEnd[2] = height - 1;
  } else {
    yStartEnd[0] = height - 1;
  }
  sendCommand(Command::SetRamYStartEnd, yStartEnd, 4);
  sendCommand(Command::SetRamYCounter, yStartEnd, 2);
}

void RPi2in13::reset() {
  gpiod_line_set_value(resetPin, 0);
  usleep(2000);
  gpiod_line_set_value(resetPin, 1);
  usleep(200000);

  init();
}

void RPi2in13::clear() {
  uint8_t clearBuf[height * width];
  std::memset(clearBuf, cfg.clearColor, height * width);

  sendCommand(Command::WriteRamBW, clearBuf, height * width);

  sendCommand(Command::DisplayUpdateControl2, 0xC7);
  sendCommand(Command::MasterActivation);

  waitForBusy();
}

void RPi2in13::sleep() {
  sendCommand(Command::DisplayUpdateControl2, 0xC3);
  sendCommand(Command::MasterActivation);

  sendCommand(Command::DeepSleepMode, 0x01);
}

void RPi2in13::setDisplayMode(DisplayMode mode) {
  if(cfg.mode != mode) {
    cfg.mode = mode;

    switch (mode)
    {
    case DisplayMode::Partial:
      setDisplayPartial();
      break;
    case DisplayMode::Full:
    default:
      setDisplayFull();
      break;
    }
  }
}

void RPi2in13::setDisplayFull() {
  sendCommand(Command::BorderWaveformControl, 0x03);

  sendCommand(Command::WriteVComRegister, 0x55);
  waitForBusy();

  sendCommand(Command::WriteLUTRegister, lut_full_update, 70);

}
void RPi2in13::setDisplayPartial() {
  sendCommand(Command::BorderWaveformControl, 0x01);

  sendCommand(Command::WriteVComRegister, 0x26);
  waitForBusy();

  sendCommand(Command::WriteLUTRegister, lut_partial_update, 70);

  uint8_t setMode2Data[] = { 0, 0, 0, 0, 0x40, 0, 0 };
  sendCommand(Command::WriteOTPSelection, setMode2Data, 7);

  sendCommand(Command::DisplayUpdateControl2, 0xC0);
  sendCommand(Command::MasterActivation);

  waitForBusy();
}

void RPi2in13::drawBuffer(Ref<uint8_t[]> buf, bool fullRefresh) {
  //  Keep a count of the Consecutive Partial Refreshes
  //  do a full refresh once its over 100;
  static auto cpr = 0;

  sendCommand(Command::WriteRamBW, buf.get(), width * height);

  if(cfg.mode == DisplayMode::Full || fullRefresh || cpr > 100) {
    cpr = 0;
    sendCommand(Command::DisplayUpdateControl2, 0xC7);
  } else {
    cpr++;
    sendCommand(Command::DisplayUpdateControl2, 0x0C);
  }

  sendCommand(Command::MasterActivation);
  waitForBusy();
}

void RPi2in13::waitForBusy() {
  while(gpiod_line_get_value(busyPin)) {
    usleep(10);
  }
}

void RPi2in13::sendCommand(Command cmd, const uint8_t* data, size_t size) {
  //  Manually controlling cs pin because this chip has an odd
  //  way of defining command byte from data bytes.
  //  DC pin must be held low during the command byte and then
  //  held high during the data bytes.
  //
  //  Alternatively, the data buffer could be turned into a
  //  uint16_t holding 9 bit words. In this case the MSB
  //  determines whether the subsequent 8 bits are a command
  //  byte (MSB 0) or a data byte (MSB 1).
  //
  //  I'm not sure which of the two options is faster or
  //  if it even matters in practice.

  gpiod_line_set_value(dcPin, 0);
  gpiod_line_set_value(csPin, 0);
  spi.write(reinterpret_cast<uint8_t*>(&cmd), 1);

  gpiod_line_set_value(dcPin, 1);
  spi.write(data, size);
  gpiod_line_set_value(csPin, 1);
}

void RPi2in13::sendCommand(Command cmd, const uint8_t data) {
  gpiod_line_set_value(dcPin, 0);
  gpiod_line_set_value(csPin, 0);
  spi.write(reinterpret_cast<uint8_t*>(&cmd), 1);

  gpiod_line_set_value(dcPin, 1);
  spi.write(&data, 1);
  gpiod_line_set_value(csPin, 1);
}

void RPi2in13::sendCommand(Command cmd) {
  gpiod_line_set_value(dcPin, 0);
  gpiod_line_set_value(csPin, 0);
  spi.write(reinterpret_cast<uint8_t*>(&cmd), 1);
  gpiod_line_set_value(csPin, 1);
}


/**
 *
 * Configuration helper Functions
 *
 */

inline uint8_t GateDrivingVoltage(float voltage) {
  if(voltage < 10.0f || voltage > 21.0f) {
    return 0x19; // return POR voltage if input is out of bounds
  }
  return static_cast<uint8_t>((voltage * 2.0f - 20.0f) + 0.5f) + 0x03;
}

uint8_t VSHVoltage(float voltage) {
  if(voltage < 9.0f) {
    return VshVoltageLow(voltage);
  } else {
    return VshVoltageHigh(voltage);
  }
}

uint8_t VshVoltageLow(float voltage) {
  int16_t tmp = static_cast<int16_t>((voltage * 10.0f - 24.0f) + 0.5f);
  if(tmp < 0) {
    return 0x8E;
  } else if(tmp > 88 - 24) {
    return 0xCE;
  }

  return tmp + 0x8E;
}

uint8_t VshVoltageHigh(float voltage) {
  int16_t tmp = static_cast<int16_t>((voltage * 5.0f - 45.0f) + 0.5f);
  if(tmp < 0) {
    return 0x23;
  } else if(tmp > 18 * 5 - 45) {
    return 0x50;
  }

  return tmp + 0x23;
}

uint8_t VslVoltage(float voltage) {
  int16_t tmp = static_cast<int16_t>((-1.0f * voltage * 2.0f - 18.0f) + 0.5f);
  if(tmp < 0) {
    return 0x1A;
  } else if(tmp > 18) {
    return 0x3E;
  }

  return tmp * 2 + 0x1A;
}

uint8_t dummyLineFromHz(uint8_t hz) {
  //  I can't tell how this correlates to the setting
  //  so this is just a long switch ladder.
  //  An enum might be better
  switch (hz) {
    case 15:
      return 0x7E;
    case 20:
      return 0x14;
    case 25:
      return 0x2A;
    case 30:
      return 0x52;
    case 40:
      return 0x47;
    case 50:
      return 0x30;
    case 60:
    case 100:
      return 0x25;
    case 70:
      return 0x2C;
    case 80:
      return 0x01;
    case 90:
      return 0x0C;
    case 110:
    case 150:
      return 0x07;
    case 120:
      return 0x18;
    case 130:
      return 0x35;
    case 140:
      return 0x1C;
    case 160:
    case 190:
      return 0x20;
    case 170:
      return 0x0D;
    case 180:
      return 0x33;
    case 200:
      return 0x10;
    default:
      return 0x30;
      break;
  }
}

uint8_t gateLineFromHz(uint8_t hz) {
  //  Same as above
  switch (hz) {
    case 15:
    case 20:
      return 0x0E;
    case 25:
      return 0x0D;
    case 30:
      return 0x0C;
    case 40:
      return 0x0B;
    case 50:
      return 0x0A;
    case 60:
      return 0x09;
    case 70:
    case 80:
      return 0x08;
    case 90:
      return 0x07;
    case 100:
    case 110:
      return 0x06;
    case 120:
      return 0x05;
    case 130:
    case 140:
    case 150:
      return 0x04;
    case 160:
    case 170:
      return 0x03;
    case 180:
    case 190:
    case 200:
      return 0x02;
    default:
      return 0x0A;
      break;
  }
}

}
