#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

#define CHART A0
#define TEMP A1
#define WARN A2
#define CONSOLES A4
#define INST_PNL A3
#define FLOOD A5
#define FLIR_1 30
#define FLIR_2 32
#define LTDR 11
#define LTDR_NFLR 10
#define NVG_1 8
#define NVG_2 9
#define MODE_1 42
#define MODE_2 43
#define LT_TEST 7
#define ANTI_ICE_PITOT 6
#define ANTI_ICE_ENG_1 5
#define ANTI_ICE_ENG_2 4
#define CABIN_PRESS_1 3
#define CABIN_PRESS_2 2
#define RGEN 26
#define LGEN 24
#define BATT 22

#define RADAR_NAV 25
#define RADAR_IFA 23
#define RADAR_GND 14
#define RADAR_CV 41
#define RADAR_OFF 40
#define RADAR_TEST 27
#define RADAR_GB 12
#define RADAR_GYRO 13

#define RADAR_STATE_OFF A8
#define RADAR_STATE_STBY A11
#define RADAR_STATE_OPR A10
#define RADAR_STATE_PULL_EMERG A9

#define BLEED_AIR_L_OFF 52
#define BLEED_AIR_NORM 48
#define BLEED_AIR_R_OFF 51
#define BLEED_AIR_OFF 53

#define CANOPY_UP 35
#define CANOPY_DOWN 33

const byte radarSwPins[4] = {RADAR_STATE_OFF, RADAR_STATE_STBY, RADAR_STATE_OPR, RADAR_STATE_PULL_EMERG};
DcsBios::SwitchMultiPos radarSw("RADAR_SW", radarSwPins, 4);

const byte insSwPins[8] = {RADAR_OFF, RADAR_CV, RADAR_GND, RADAR_NAV, RADAR_IFA, RADAR_GYRO, RADAR_GB, RADAR_TEST};
DcsBios::SwitchMultiPos insSw("INS_SW", insSwPins, 8);

DcsBios::Potentiometer floodDimmer("FLOOD_DIMMER", FLOOD);
DcsBios::Potentiometer chartDimmer("CHART_DIMMER", CHART);
DcsBios::Potentiometer consolesDimmer("CONSOLES_DIMMER", CONSOLES);
DcsBios::Potentiometer instPnlDimmer("INST_PNL_DIMMER", INST_PNL);
DcsBios::Potentiometer warnCautionDimmer("WARN_CAUTION_DIMMER", WARN);
DcsBios::Potentiometer cabinTemp("CABIN_TEMP", TEMP);

DcsBios::RotaryEncoder flirSw("FLIR_SW", "DEC", "INC", FLIR_2, FLIR_1);
DcsBios::Switch2Pos ltdRSw("LTD_R_SW", LTDR, true);
DcsBios::Switch2Pos lstNflrSw("LST_NFLR_SW", LTDR_NFLR, true);
DcsBios::RotaryEncoder batterySw("BATTERY_SW", "DEC", "INC", -1 , BATT);
DcsBios::Switch2Pos lGenSw("L_GEN_SW", LGEN, true);
DcsBios::Switch2Pos rGenSw("R_GEN_SW", RGEN);

const byte bleedAirKnobPins[4] = {BLEED_AIR_OFF, BLEED_AIR_L_OFF, BLEED_AIR_NORM,BLEED_AIR_R_OFF};
DcsBios::SwitchMultiPos bleedAirKnob("BLEED_AIR_KNOB", bleedAirKnobPins, 4);

DcsBios::Switch3Pos canopySw("CANOPY_SW", CANOPY_UP, CANOPY_DOWN);

DcsBios::RotaryEncoder cockkpitLightModeSw("COCKKPIT_LIGHT_MODE_SW", "DEC", "INC", NVG_2, NVG_1);
DcsBios::RotaryEncoder ecsModeSw("ECS_MODE_SW", "DEC", "INC", MODE_1, MODE_2);
DcsBios::RotaryEncoder cabinPressSw("CABIN_PRESS_SW", "DEC", "INC", CABIN_PRESS_1, CABIN_PRESS_2);
DcsBios::Switch2Pos pitotHeatSw("PITOT_HEAT_SW", ANTI_ICE_PITOT);
DcsBios::RotaryEncoder engAntiiceSw("ENG_ANTIICE_SW", "DEC", "INC", ANTI_ICE_ENG_1, ANTI_ICE_ENG_2);
DcsBios::Switch2Pos lightsTestSw("LIGHTS_TEST_SW", LT_TEST, true);


void setup() {
  DcsBios::setup();
  pinMode(CHART, INPUT);
  pinMode(TEMP, INPUT);
  pinMode(WARN, INPUT);
  pinMode(CONSOLES, INPUT);
  pinMode(INST_PNL, INPUT);
  pinMode(FLOOD, INPUT);
  pinMode(FLIR_1, INPUT_PULLUP);
  pinMode(FLIR_2, INPUT_PULLUP);
  pinMode(LTDR, INPUT_PULLUP);
  pinMode(LTDR_NFLR, INPUT_PULLUP);
  pinMode(MODE_1, INPUT_PULLUP);
  pinMode(MODE_2, INPUT_PULLUP);
  pinMode(NVG_1, INPUT_PULLUP);
  pinMode(NVG_2, INPUT_PULLUP);
  pinMode(LT_TEST, INPUT_PULLUP);
  pinMode(ANTI_ICE_PITOT, INPUT_PULLUP);
  pinMode(ANTI_ICE_ENG_1, INPUT_PULLUP);
  pinMode(ANTI_ICE_ENG_2, INPUT_PULLUP);
  pinMode(CABIN_PRESS_1, INPUT_PULLUP);
  pinMode(CABIN_PRESS_2, INPUT_PULLUP);
  pinMode(RGEN, INPUT_PULLUP);
  pinMode(LGEN, INPUT_PULLUP);
  pinMode(BATT, INPUT_PULLUP);

  pinMode(RADAR_OFF, INPUT_PULLUP);
  pinMode(RADAR_GND, INPUT_PULLUP);
  pinMode(RADAR_CV, INPUT_PULLUP);
  pinMode(RADAR_NAV, INPUT_PULLUP);
  pinMode(RADAR_IFA, INPUT_PULLUP);
  pinMode(RADAR_GYRO, INPUT_PULLUP);
  pinMode(RADAR_GB, INPUT_PULLUP);
  pinMode(RADAR_TEST, INPUT_PULLUP);

  pinMode(RADAR_STATE_OFF, INPUT_PULLUP);
  pinMode(RADAR_STATE_STBY, INPUT_PULLUP);
  pinMode(RADAR_STATE_OPR, INPUT_PULLUP);
  pinMode(RADAR_STATE_PULL_EMERG, INPUT_PULLUP);

  pinMode(BLEED_AIR_L_OFF, INPUT_PULLUP);
  pinMode(BLEED_AIR_R_OFF, INPUT_PULLUP);
  pinMode(BLEED_AIR_OFF, INPUT_PULLUP);
  pinMode(BLEED_AIR_NORM, INPUT_PULLUP);
  
  pinMode(CANOPY_UP, INPUT_PULLUP);
  pinMode(CANOPY_DOWN, INPUT_PULLUP);
}


void loop() {
    DcsBios::loop();
  /**
    Serial.println("POTENTIOMETERS");
    Serial.print(analogRead(CHART));
    Serial.print(" - ");
    Serial.print(analogRead(TEMP));
    Serial.print(" - ");
    Serial.print(analogRead(WARN));
    Serial.print(" - ");
    Serial.print(analogRead(CONSOLES));
    Serial.print(" - ");
    Serial.print(analogRead(INST_PNL));
    Serial.print(" - ");
    Serial.println(analogRead(FLOOD));

    Serial.println("-----------------------------------------");
    Serial.println("-----------------------------------------");
    Serial.println("-----------------------------------------");

    Serial.println("BUTTONS");
    Serial.print("FLIR: ");
    Serial.print(digitalRead(FLIR_1));
    Serial.print(digitalRead(FLIR_2));
    Serial.print(" - LTDR: ");
    Serial.print(digitalRead(LTDR));
    Serial.print(" - LTDR_NFLR: ");
    Serial.print(digitalRead(LTDR_NFLR)); 
    Serial.print(" - MODE: ");
    Serial.print(digitalRead(MODE_1)); 
    Serial.print(digitalRead(MODE_2));   
    Serial.print(" - NVG: ");
    Serial.print(digitalRead(NVG_1)); 
    Serial.print(digitalRead(NVG_2));  
    Serial.print(" - LT_TEST: ");
    Serial.print(digitalRead(LT_TEST));  
    Serial.print(" - ANTI_ICE_PITOT: ");
    Serial.print(digitalRead(ANTI_ICE_PITOT));
    Serial.print(" - ANTI_ICE_ENG: ");
    Serial.print(digitalRead(ANTI_ICE_ENG_1));
    Serial.print(digitalRead(ANTI_ICE_ENG_2));
    Serial.print(" - CABIN_PRESS: ");
    Serial.print(digitalRead(CABIN_PRESS_1));
    Serial.print(digitalRead(CABIN_PRESS_2));
    Serial.print(" - RGEN: ");
    Serial.print(digitalRead(RGEN));
    Serial.print(" - LGEN: ");
    Serial.print(digitalRead(LGEN));
    Serial.print(" - BATT: ");
    Serial.println(digitalRead(BATT));
  **/
    /**
    Serial.print(digitalRead(RADAR_OFF));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_CV));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_GND));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_NAV));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_IFA));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_GYRO));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_GB)); 
    Serial.print(" - ");
    Serial.println(digitalRead(RADAR_TEST));
  */
    /**
    Serial.print(digitalRead(RADAR_STATE_OFF));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_STATE_STBY));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_STATE_OPR));
    Serial.print(" - ");
    Serial.print(digitalRead(RADAR_STATE_PULL_EMERG));
    Serial.print(" - ");
    Serial.print(" - ");
    Serial.print(digitalRead(BLEED_AIR_L_OFF));
    Serial.print(" - ");
    Serial.print(digitalRead(BLEED_AIR_NORM));
    Serial.print(" - ");
    Serial.print(digitalRead(BLEED_AIR_R_OFF)); 
    Serial.print(" - ");
    Serial.println(digitalRead(BLEED_AIR_OFF));
    */
      /*
      Serial.print(" - ");
    Serial.print(digitalRead(CANOPY_DOWN)); 
    Serial.print(" - ");
    Serial.println(digitalRead(CANOPY_UP));
  */
  }
