#include "Bounce2.h"

#define TEENSY 1
 
#if TEENSY == 0
#include "MidiUSB.h"
#endif

// inputs
const int panic = 5;
const int go = 6;
const int stp = 7;
const int util = 8;

// outputs
const int stOut1 = 0;
const int stOut2 = 1;

Bounce panicBn;
Bounce goBn;
Bounce stpBn;
Bounce utilBn;

void setup() {
  pinMode(panic, INPUT_PULLUP);
  pinMode(go, INPUT_PULLUP);
  pinMode(stp, INPUT_PULLUP);
  pinMode(util, INPUT_PULLUP);
  pinMode(stOut1, OUTPUT);
  pinMode(stOut2, OUTPUT);

  panicBn.attach(panic);
  goBn.attach(go);
  stpBn.attach(stp);
  utilBn.attach(util);

  red();
}

void loop() {
  handleBtn(&panicBn, panic, 113);
  handleBtn(&goBn, go, 111);
  handleBtn(&utilBn, util, 114);
  handleBtn(&stpBn, stp, 112);

#if TEENSY
  usbMIDI.send_now();
#else
  MidiUSB.flush();
#endif
}

void handleBtn(Bounce *btn, int port, int Midi) {
  btn->update();
  if (btn->fell()) {
    controlChange(8, Midi, 127);
    green();
  }else if (btn->rose()) {
    controlChange(8, Midi, 0);
    red();
  }
}

#if TEENSY
void controlChange(byte channel, byte control, byte value) {
  usbMIDI.sendControlChange(control, value, channel);
}
#else
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
#endif

void red(){
  digitalWrite(stOut1, HIGH);
  digitalWrite(stOut2, LOW);
}

void green(){
  digitalWrite(stOut2, HIGH);
  digitalWrite(stOut1, LOW);
}
