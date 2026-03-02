#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
// === НАСТРОЙКИ Wi-Fi ===
const char* ssid = "TP-Link_00E0";      // Замените на имя вашей Wi-Fi сети
const char* password = "68860097"; // Замените на пароль

// === ПИНЫ ===
const int ledPin = 8;  // GPIO 8 на большинстве ESP32-C3 (можно изменить)
bool ledState = false; // Текущее состояние светодиода

// === СОЗДАЕМ ВЕБ-СЕРВЕР НА ПОРТУ 80 ===
WebServer server(80);

// === ОБРАБОТЧИК ГЛАВНОЙ СТРАНИЦЫ ===
void handleRoot() {
  // Создаем HTML-страницу БЕЗ СТИЛЕЙ
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>ESP32-C3 Управление</title>
</head>
<body>
    
    <h3>Статус светодиода: <strong>)rawliteral";
  
  html += ledState ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН";
  
  html += R"rawliteral(</strong></h3>
    
    <p>
        <a href="/on"><button>ВЫКЛЮЧИТЬ</button></a>
        <a href="/off"><button>ВКЛЮЧИТЬ</button></a>
    </p>
    
    <p>
  
</small>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}
// === ОБРАБОТЧИК ВКЛЮЧЕНИЯ ===
void handleOn() {
  ledState = true;
  digitalWrite(ledPin, HIGH);
  Serial.println(" Светодиод ВКЛЮЧЕН");
  
  // Отправляем обновленную главную страницу
  handleRoot();
}

// === ОБРАБОТЧИК ВЫКЛЮЧЕНИЯ ===
void handleOff() {
  ledState = false;
  digitalWrite(ledPin, LOW);
  Serial.println(" Светодиод ВЫКЛЮЧЕН");
  
  // Отправляем обновленную главную страницу
  handleRoot();
}

// === ОБРАБОТЧИК ДЛЯ API (JSON) ===
void handleApi() {
  String json = "{\"ledState\":";
  json += ledState ? "true" : "false";
  json += ",\"uptime\":";
  json += String(millis() / 1000);
  json += "}";
  
  server.send(200, "application/json", json);
}

// === ОБРАБОТЧИК 404 (страница не найдена) ===
void handleNotFound() {
  server.send(404, "text/plain", "404: Страница не найдена");
}

// === НАСТРОЙКА ===
void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=== ESP32-C3 ВЕБ-СЕРВЕР ===");
  
  // Настройка пина светодиода
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Подключение к Wi-Fi
  Serial.print("Подключение к Wi-Fi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // Ждем подключения с таймаутом
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n Wi-Fi подключен!");
    Serial.print(" IP адрес: ");
    Serial.println(WiFi.localIP());
    Serial.print(" Подключитесь по адресу: http://");
    Serial.println(WiFi.localIP().toString());
  } else {
    Serial.println("\n Ошибка подключения к Wi-Fi!");
    Serial.println("Проверьте SSID и пароль в коде.");
  }
  
  // Настройка маршрутов веб-сервера
  server.on("/", handleRoot);           // Главная страница
  server.on("/on", handleOn);            // Включить LED
  server.on("/off", handleOff);          // Выключить LED
  server.on("/api", handleApi);          // API для JSON данных
  server.onNotFound(handleNotFound);     // Страница 404
  
  // Запуск сервера
  server.begin();
  Serial.println(" Веб-сервер запущен!");
}

// === ГЛАВНЫЙ ЦИКЛ ===
void loop() {
  server.handleClient(); // Обрабатываем запросы клиентов
  
  // Дополнительно: можно добавить мигание для индикации работы
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 5000) {
    lastBlink = millis();
  }
  
  delay(10);
}