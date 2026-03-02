#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include <html/pages.h>
#include <app_state.h>
#include <config.h>

AppState appState;

WebServer server(SERVER_PORT);

void handleRoot()
{
  String html = renderMainPage(appState);
  server.send(200, "text/html", html);
}

void handleOn()
{
  appState.ledOn = true;
  digitalWrite(LED_PIN, HIGH);
  Serial.println(" Светодиод ВКЛЮЧЕН");

  handleRoot();
}

void handleOff()
{
  appState.ledOn = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println(" Светодиод ВЫКЛЮЧЕН");

  handleRoot();
}

void handleApi()
{
  String json = "{\"ledState\":";
  json += appState.ledOn ? "true" : "false";
  json += ",\"uptime\":";
  json += String(millis() / 1000);
  json += "}";

  server.send(200, "application/json", json);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Страница не найдена");
}

void saveConfigCallback() 
{
  Serial.println("Настройки Wi-Fi сохранены!");
}

void setup()
{
  Serial.begin(115200);

  Serial.println("\n\n=== ESP32-C3 ВЕБ-СЕРВЕР ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFiManager wm;
  wm.setSaveConfigCallback(saveConfigCallback);
  //wm.resetSettings();
  bool connected = wm.autoConnect("ESP32-C3-Config", "12345678");
  
  if(!connected) {
    Serial.println("Не удалось подключиться к Wi-Fi");
    delay(10000);
    ESP.restart();
  }

  Serial.println("\n Wi-Fi подключен через WiFiManager");
  Serial.print(" IP адрес: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);        // Главная страница
  server.on("/on", handleOn);        // Включить LED
  server.on("/off", handleOff);      // Выключить LED
  server.on("/api", handleApi);      // API для JSON данных
  server.onNotFound(handleNotFound); // Страница 404

  server.begin();
  Serial.println(" Веб-сервер запущен!");
}


void loop()
{
  server.handleClient();
  delay(10);
}