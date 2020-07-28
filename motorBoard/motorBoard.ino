#include "utility.h"
#include "pins.h"
#include "dc_motor.h"
#include "motorArray.h"

#define CMD_ROT_RESET_POS       30
#define CMD_ROT_DIR_ANGLE       31
#define CMD_ROT_ROTATE          32
#define CMD_ROT_DIR_DANGLE      33

// Serial ports
#define SonarSerial Serial4
#define T0Serial    Serial1
#define Debug       Serial

void updateA(Motor motor) { motor.updateA(); }
void updateB(Motor motor) { motor.updateB(); }

void setup() {
  //  Debug.begin(115200); // Debug serial port
  T0Serial.begin(115200); // TMain communication
  Serial.begin(115200);
  motors.setup();

  Motor M1();
  Motor M2();
  Motor M3();
  Motor M4();

  attachInterrupt(digitalPinToInterrupt(M1.getPinA()), updateA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M1.getPinB()), updateB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M2.getPinA()), updateA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M2.getPinB()), updateB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M3.getPinA()), updateA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M3.getPinB()), updateB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M4.getPinA()), updateA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M4.getPinB()), updateB, CHANGE);

  delay(1000);
  //  motors.resetPosition();
  Serial.println("Setup Done");
}

void loop() {
  if(T0Serial.available() > 0) {
    uint8_t cmd;
    SerialRead<uint8_t>(T0Serial, &cmd);
    Serial.println(cmd);
    switch (cmd) {
      case CMD_ROT_RESET_POS: {
        Serial.println("CMD_ROT_RESET_POS");
        Debug.println("Received resetPos");
        motors.resetPosition(); // blocking here, T0 should wait untill we're done
        SerialWriteOK(T0Serial);
        break;
      }
      case CMD_ROT_DIR_ANGLE: {
        Serial.println("DirAngle");
        float angleL, angleR;
        SerialRead<float>(T0Serial, &angleL);
        SerialRead<float>(T0Serial, &angleR);
        motors.angleDir(angleL, angleR);
        break;
      }
      case CMD_ROT_DIR_DANGLE: {
        float dangle[2];
        SerialRead<float>(T0Serial, dangle, 2);
        motors.deltaAngleDir(dangle[0], dangle[1]);
        break;
      }

      case CMD_ROT_ROTATE: {
        motors.rotatePos(); // blocking here, T0 should wait untill we're done
        SerialWriteOK(T0Serial);
        break;
      }
    }
  }
  //Debug.println("update");
  motors.update();

  delay(1);
}
