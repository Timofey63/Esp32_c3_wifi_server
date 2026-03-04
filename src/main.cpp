#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <LittleFS.h>

#include <appState.h>
#include <config.h>
#include <web/web_server.h>
AppState appState;

void setup()
{
  Serial.begin(115200);
  
  Serial.println("\n\n=== ESP32-C3 ВЕБ-СЕРВЕР ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFiManager wm;
  //wm.setSaveConfigCallback(saveConfigCallback);
  //wm.resetSettings();
  bool connected = wm.autoConnect("ESP32-C3-Config", "12345678");
  
  if(!connected) 
  {
    Serial.println("Не удалось подключиться к Wi-Fi");
    delay(10000);
    ESP.restart();
  }
  
  if (!LittleFS.begin(true)) 
  {
    Serial.println("Ошибка LittleFS");
    return;
  }

  Serial.print(" IP адрес: http://");
  Serial.println(WiFi.localIP());

  webInit();
  Serial.println(" Веб-сервер запущен!");
}

void loop()
{
  webLoop();
  delay(10);
}