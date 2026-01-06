#include <M5Stack.h>

void setup() {
  M5.begin();

  // Display QRCode
  M5.Lcd.qrcode("HELLO M5STACK");
}

void loop() {
}
