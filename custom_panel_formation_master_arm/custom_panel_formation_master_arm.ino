#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

DcsBios::Switch3Pos cmsdDispenseSw("CMSD_DISPENSE_SW", 10, 9);
DcsBios::Switch2Pos auxRelSw("AUX_REL_SW", 11, true);
DcsBios::Switch2Pos masterModeAg("MASTER_MODE_AG", 8);
DcsBios::Switch2Pos masterModeAa("MASTER_MODE_AA", 4);
DcsBios::Potentiometer formationDimmer("FORMATION_DIMMER", A7);
DcsBios::Potentiometer positionDimmer("POSITION_DIMMER", A6);
DcsBios::Switch2Pos masterArmSw("MASTER_ARM_SW", 12, true);

const byte ecmModeSwPins[5] = {A1, A2, A3, A4, A5};
DcsBios::SwitchMultiPos ecmModeSw("ECM_MODE_SW", ecmModeSwPins, 5);

void onMasterModeAgLtChange(unsigned int newValue) {
  if (newValue == 1){
      digitalWrite(13, HIGH);
  }else{
      digitalWrite(13, LOW);
  }
}

DcsBios::IntegerBuffer masterModeAgLtBuffer(0x740c, 0x0400, 10, onMasterModeAgLtChange);

void onMasterModeAaLtChange(unsigned int newValue) {
  if (newValue == 1){
      digitalWrite(3, HIGH);
  }else{
      digitalWrite(3, LOW);
  }
}
DcsBios::IntegerBuffer masterModeAaLtBuffer(0x740c, 0x0200, 9, onMasterModeAaLtChange);

void setup() {
  DcsBios::setup();

  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT);
  pinMode(4, INPUT); 
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
}     

void loop() {
  DcsBios::loop();
}
