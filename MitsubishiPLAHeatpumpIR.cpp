/*
    Mitsubishi PLA series ceiling cassette with W001CP remote

    Key differences from mitsubishi_sez:
    - Fan speed is in byte 8 (not byte 7)
    - Fan speed values: 0x04 (Low), 0x24 (Med-Low), 0x44 (Medium), 0x64 (High)
    - Supports 4 fan speeds instead of 3

    Based on IR captures from W001CP remote (R61Y23391)
*/

#include <MitsubishiPLAHeatpumpIR.h>

MitsubishiPLAHeatpumpIR::MitsubishiPLAHeatpumpIR() : HeatpumpIR()
{
  static const char model[]  PROGMEM = "mitsubishi_pla";
  static const char info[]   PROGMEM = "{\"mdl\":\"mitsubishi_pla\",\"dn\":\"Mitsubishi PLA\",\"mT\":17,\"xT\":30,\"fs\":4}";

  _model = model;
  _info = info;
}

void MitsubishiPLAHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd)
{
  // Sensible defaults for the heat pump mode
  uint8_t powerMode     = MITSUBISHIPLA_AIRCON1_MODE_ON;
  uint8_t operatingMode = MITSUBISHIPLA_AIRCON1_MODE_AUTO;
  uint8_t fanSpeed      = MITSUBISHIPLA_AIRCON1_FAN_LOW;
  uint8_t temperature   = 23;
  uint8_t swingV        = 0x00;
  uint8_t swingH        = 0x00;

  switch (powerModeCmd)
  {
    case POWER_OFF:
      powerMode = MITSUBISHIPLA_AIRCON1_MODE_OFF;
      break;
  }

  switch (operatingModeCmd)
  {
    case MODE_AUTO:
      operatingMode = MITSUBISHIPLA_AIRCON1_MODE_AUTO;
      break;
    case MODE_HEAT:
      operatingMode = MITSUBISHIPLA_AIRCON1_MODE_HEAT;
      break;
    case MODE_COOL:
      operatingMode = MITSUBISHIPLA_AIRCON1_MODE_COOL;
      break;
    case MODE_DRY:
      operatingMode = MITSUBISHIPLA_AIRCON1_MODE_DRY;
      break;
    case MODE_FAN:
      operatingMode = MITSUBISHIPLA_AIRCON1_MODE_FAN;
      break;
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = MITSUBISHIPLA_AIRCON1_FAN_LOW;  // Auto/Quiet = 0x31
      break;
    case FAN_1:
      fanSpeed = MITSUBISHIPLA_AIRCON1_FAN_LOW;  // Auto/Quiet = 0x31
      break;
    case FAN_2:
      fanSpeed = MITSUBISHIPLA_AIRCON1_FAN_MEDLOW;  // Low = 0x33
      break;
    case FAN_3:
      fanSpeed = MITSUBISHIPLA_AIRCON1_FAN_MEDIUM;  // Medium = 0x35
      break;
    case FAN_4:
      fanSpeed = MITSUBISHIPLA_AIRCON1_FAN_HIGH;  // High = 0x37
      break;
    case FAN_5:
      fanSpeed = MITSUBISHIPLA_AIRCON1_FAN_HIGH;  // High = 0x37
      break;
  }

  if (temperatureCmd > 16 && temperatureCmd < 31)
  {
    temperature = temperatureCmd;
  }

  sendMitsubishiPLA(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH);
}

void MitsubishiPLAHeatpumpIR::sendMitsubishiPLA(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH)
{
  // Template from captured W001CP remote signals
  // Byte 7 = 0x00, Byte 8 = fan speed (different from SEZ which uses byte 7 for fan)
  uint8_t MitsubishiTemplate[] = { 0x23, 0xCB, 0x26, 0x21, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //                                  0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16

  // Set the power mode on the template message
  MitsubishiTemplate[5] = powerMode;

  // Set the temperature and operating mode on the template message
  MitsubishiTemplate[6] = ((temperature - 16) << 4) | operatingMode;

  // Set the fan speed in byte 7 (same position as SEZ, different values)
  MitsubishiTemplate[7] = fanSpeed;
  // Byte 8 stays 0x04 (from template)

  // Checksum bytes (inverted copies of bytes 5-10)
  MitsubishiTemplate[11] = ~MitsubishiTemplate[5];
  MitsubishiTemplate[12] = ~MitsubishiTemplate[6];
  MitsubishiTemplate[13] = ~MitsubishiTemplate[7];
  MitsubishiTemplate[14] = ~MitsubishiTemplate[8];
  MitsubishiTemplate[15] = ~MitsubishiTemplate[9];
  MitsubishiTemplate[16] = ~MitsubishiTemplate[10];

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  IR.mark(MITSUBISHIPLA_AIRCON1_HDR_MARK);
  IR.space(MITSUBISHIPLA_AIRCON1_HDR_SPACE);

  // Data
  for (unsigned int i = 0; i < sizeof(MitsubishiTemplate); i++) {
    IR.sendIRbyte(MitsubishiTemplate[i], MITSUBISHIPLA_AIRCON1_BIT_MARK, MITSUBISHIPLA_AIRCON1_ZERO_SPACE, MITSUBISHIPLA_AIRCON1_ONE_SPACE);
  }

  // End mark
  IR.mark(MITSUBISHIPLA_AIRCON1_BIT_MARK);
  IR.space(0);
}
