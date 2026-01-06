#define BLYNK_PRINT Serial
#include <M5Stack.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleDHT.h>

char auth[] = "ee8a05c5519b4fe1851f08373c93badb";
char ssid[] = "plutter";
char pass[] = "visit012";

BlynkTimer timer;

int DHTPin = 5;
SimpleDHT11 dht11;


BLYNK_WRITE(V3) 
{
  if (param.asInt() == 1) {
      if (digitalRead(2) != 1) {
          digitalWrite(2, HIGH);
          M5.Lcd.fillScreen(RED);
      }
  } else {
        digitalWrite(2, LOW);
        M5.Lcd.fillScreen(BLACK);
        M5.Speaker.beep(); //beep
  }
}

void sendSensor() {

  byte temperature = 0;           //กำหนดค่าตัวแปรอุณหภูมิ
  byte humidity = 0;              //กำหนดค่าความชื้นสัมสัทธ์
  int count = 1;
  
  //อ่านค่าจากเซ็นเซอร์ DHT11
  dht11.read(DHTPin, &temperature, &humidity, NULL);
  
  //ถ้าค่าอุณหภูมิ = 0 ให้อ่านค่าจากเซ็นเซอร์ใหม่ 4 ครั้ง
  while (temperature == 0 && count <= 4) {
    
    //แสดงข้อความใน Serial Monitor
    //Serial.println(", DHT11 get failed ");
    //Serial.print(count);
    
    //อ่านค่าจากเซ็นเซอร์ DHT11
    dht11.read(DHTPin, &temperature, &humidity, NULL);
    delay(1000);
    count++;
  }
  
  Blynk.virtualWrite(V1, humidity);     //ส่งค่า humidity V1 ไปยัง blynk server
  Blynk.virtualWrite(V2, temperature);  //ส่งค่า humitemperature V2 ไปยัง blynk server

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(4);
  M5.Lcd.print("Tem: ");
  M5.Lcd.print(temperature);
  M5.Lcd.println(" C");
  M5.Lcd.print("Hum: ");
  M5.Lcd.print(humidity);
  M5.Lcd.println(" %");

}

void setup() {

  pinMode(2, OUTPUT);
  M5.begin();

  // Set the wakeup button
  M5.setWakeupButton(BUTTON_A_PIN);
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, IPAddress(128,199,204,127), 8080);
  timer.setInterval(5000L, sendSensor);

  M5.Lcd.setBrightness(10);
  
}

void loop() {
  
  Blynk.run();
  timer.run();

  if(M5.BtnA.wasPressed()) {
    M5.powerOFF();
  }

  M5.update();
 
}
