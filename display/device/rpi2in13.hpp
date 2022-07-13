#ifndef RPI2IN13_HPP
#define RPI2IN13_HPP

#include <cstdint>
#include <gpiod.h>

#include <spidev.hpp>

#include "../display_device.hpp"


namespace EPD {

class RPi2in13: public DisplayDevice
{
public:

  RPi2in13(DisplayConfig);
  ~RPi2in13();

  void init();

  inline int getWidth() { return width; }
  inline int getHeight() { return height; }

  void reset();
  void clear();
  void sleep();

  void setDisplayMode(DisplayMode);
  void drawBuffer(Ref<uint8_t[]> buf, bool fullRefresh = false);
private:

  enum class Command: uint8_t {
    DriverOutput                = 0x01,
    GateDrivingVoltageControl   = 0x03,
    SourceDrivingVoltageControl = 0x04,
    BoosterSoftStartControl     = 0x0C,
    GateScanStartPosition       = 0x0F,
    DeepSleepMode               = 0x10,
    DataEntryModeSetting        = 0x11,
    SoftwareReset               = 0x12,
    HVReadyDetection            = 0x14,
    VCIDetection                = 0x14,
    TempSensorWrite             = 0x1A,
    TempSensorRead              = 0x1B,
    TempSensorCMD               = 0x1C,
    MasterActivation            = 0x20,
    DisplayUpdateControl1       = 0x21,
    DisplayUpdateControl2       = 0x22,
    WriteRamBW                  = 0x24,
    WriteRamRed                 = 0x26,
    ReadRam                     = 0x27,
    VComSense                   = 0x28,
    VComSenseDuration           = 0x29,
    ProgramVComOTP              = 0x2A,
    WriteVComRegister           = 0x2C,
    OTPRegisterRead             = 0x2D,
    StatusBitRead               = 0x2F,
    ProgramWsOTP                = 0x30,
    LoadWsOTP                   = 0x31,
    WriteLUTRegister            = 0x32,
    ProgramOTPSelection         = 0x36,
    WriteOTPSelection           = 0x37,
    SetDummyLinePeriod          = 0x3A,
    SetGateLineWidth            = 0x3B,
    BorderWaveformControl       = 0x3C,
    ReadRamOption               = 0x41,
    SetRamXStartEnd             = 0x44,
    SetRamYStartEnd             = 0x45,
    RedRamWritePattern          = 0x46,
    BWRamWritePattern           = 0x47,
    SetRamXCounter              = 0x4E,
    SetRamYCounter              = 0x4F,
    SetAnalogBlockControl       = 0x74,
    SetDigitalBlockControl      = 0x7E,
    NOP                         = 0x7F
  };

  void initRamSettings();

  void setDisplayFull();
  void setDisplayPartial();

  void sendCommand(Command, const uint8_t*, size_t);
  void sendCommand(Command, const uint8_t);
  void sendCommand(Command);
  void waitForBusy();

  Util::SPI spi;

  uint8_t width;
  uint16_t height;

  gpiod_chip* gpio;
  gpiod_line* csPin;
  gpiod_line* dcPin;
  gpiod_line* busyPin;
  gpiod_line* resetPin;

};


}

#endif /* RPI2IN13_HPP */
