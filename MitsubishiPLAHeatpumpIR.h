/*
    Mitsubishi PLA series ceiling cassette with W001CP remote

    Fan speed is in byte 8 (not byte 7 like SEZ variant)
    Fan speed values: 0x04 (Low), 0x24 (Med-Low), 0x44 (Medium), 0x64 (High)

    Based on IR captures from W001CP remote (R61Y23391)
*/

#ifndef MitsubishiPLAHeatpumpIR_h
#define MitsubishiPLAHeatpumpIR_h

#include <HeatpumpIR.h>

// Mitsubishi PLA timing constants (same as SEZ)
#define MITSUBISHIPLA_AIRCON1_HDR_MARK   3060
#define MITSUBISHIPLA_AIRCON1_HDR_SPACE  1580
#define MITSUBISHIPLA_AIRCON1_BIT_MARK   350
#define MITSUBISHIPLA_AIRCON1_ONE_SPACE  1150
#define MITSUBISHIPLA_AIRCON1_ZERO_SPACE 390
#define MITSUBISHIPLA_AIRCON1_MSG_SPACE  0

// Mitsubishi PLA operating modes
#define MITSUBISHIPLA_AIRCON1_MODE_AUTO      0x03
#define MITSUBISHIPLA_AIRCON1_MODE_HEAT      0x02
#define MITSUBISHIPLA_AIRCON1_MODE_COOL      0x01
#define MITSUBISHIPLA_AIRCON1_MODE_DRY       0x05
#define MITSUBISHIPLA_AIRCON1_MODE_FAN       0x00
#define MITSUBISHIPLA_AIRCON1_MODE_OFF       0x00
#define MITSUBISHIPLA_AIRCON1_MODE_ON        0x40

// Mitsubishi PLA fan speeds (byte 8, captured from W001CP remote)
#define MITSUBISHIPLA_AIRCON1_FAN_LOW        0x04  // FAN_1
#define MITSUBISHIPLA_AIRCON1_FAN_MEDLOW     0x24  // FAN_2
#define MITSUBISHIPLA_AIRCON1_FAN_MEDIUM     0x44  // FAN_3
#define MITSUBISHIPLA_AIRCON1_FAN_HIGH       0x64  // FAN_4

class MitsubishiPLAHeatpumpIR : public HeatpumpIR
{
  public:
    MitsubishiPLAHeatpumpIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd);

  private:
    void sendMitsubishiPLA(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH);
};

#endif
