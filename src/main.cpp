#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <html/pages.h>
#include <app_state.h>
#include <config.h>

AppState appState;
const char *ssid = "TP-Link_00E0";
const char *password = "68860097";

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


void setup()
{
  Serial.begin(115200);
  delay(100);

  Serial.println("\n\n=== ESP32-C3 ВЕБ-СЕРВЕР ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.print("Подключение к Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n Wi-Fi подключен!");
    Serial.print(" IP адрес: ");
    Serial.println(WiFi.localIP());
    Serial.print(" Подключитесь по адресу: http://");
    Serial.println(WiFi.localIP().toString());
  }
  else
  {
    Serial.println("\n Ошибка подключения к Wi-Fi!");
    Serial.println("Проверьте SSID и пароль в коде.");
  }

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