#include <USBHost_t36.h>
#include <ps4bt.h>

USBHost usbHost;
BluetoothController bluet(usbHost); 
PS4BT ps4BT(usbHost);

IntervalTimer blinkTimer;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  usbHost.begin();
  blinkTimer.begin(blinkLED, 1000000);
  delay(2000);
  Serial.printf("PS4BTSample....\n");
}

int ledState = LOW;
void blinkLED() { 
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(LED_BUILTIN, ledState);
}


void loop() {
  usbHost.Task();
  if (ps4BT.available()) {
     uint32_t buttons = ps4BT.getButtons();
     Serial.printf("Buttons: %x\r\n", buttons);
//    Serial.printf("Joystick Available\n");
//    ps4BT.dump();    
//    ps4BT.clearData();
  }
  delay(1500);
}
