#include <Nefry.h>
#include <NefryDisplay.h>
#include <NefrySetting.h>
#include <misakiUTF16.h>
#include "DHTesp.h"

DHTesp dht;

void setting(){
  Nefry.disableDisplayStatus();
}

NefrySetting nefrySetting(setting);

void misakiPrint(int x, int y, char * pUTF8) {
  int n=0;
  byte buf[20][8];  //160x8ドットのバナー表示パターン
  while(*pUTF8)
    pUTF8 = getFontData(&buf[n++][0], pUTF8);  // フォントデータの取得

  // 文字の描画
  for (byte i=0; i < 8; i++) {
    for (byte j=0; j < n; j++) {
      for (byte k=0; k<8;k++) {
        if(bitRead(buf[j][i],7-k)) {
          NefryDisplay.setPixel(x + 8*j + k , y + i);
        }
      }
    }
  }
}

void displayInfo() {
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
    Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("humidity: ");
    Serial.println(humidity);
    NefryDisplay.clear();
    NefryDisplay.setFont(ArialMT_Plain_24);
    misakiPrint(0,0, "気温:");
    misakiPrint(0,20, "湿度:");
    misakiPrint(75,10, "℃");
    misakiPrint(75,30, "%");
    char s_temperature[8];
    dtostrf(temperature, 2, 1, s_temperature);
    char s_humidity[8];
    dtostrf(humidity, 2, 1, s_humidity);
    NefryDisplay.drawString(25, 0, s_temperature);
    NefryDisplay.drawString(25, 20, s_humidity);
    NefryDisplay.display();
    if (humidity < 30) {
      Nefry.setLed(255, 0, 0);
      Serial.println("Water Atomization: ON");
      digitalWrite(A2, HIGH);
    } else {
      Nefry.setLed(0, 255, 0);
      digitalWrite(A2, LOW);
      Serial.println("Water Atomization: OFF");
    }
}

void setup() {
  Serial.begin(115200);
  dht.setup(25); // A0:IO25
  pinMode(A2, OUTPUT); // A2:Grove Water Atomization
  NefryDisplay.clear();
  NefryDisplay.setFont(ArialMT_Plain_24);
  misakiPrint(0,0, "気温:");
  misakiPrint(0,20, "湿度:");
  misakiPrint(75,10, "℃");
  misakiPrint(75,30, "%");
  NefryDisplay.display();
}

void loop() {
  delay(dht.getMinimumSamplingPeriod());
  displayInfo();
  delay(100);
}

