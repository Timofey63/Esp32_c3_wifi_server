#include <Arduino.h>
#include <WebServer.h>

#include <html/pages.h>
#include <appState.h>
#include <config.h>

static WebServer server(SERVER_PORT);

static void handleRoot()
{
    server.send(200, "text/html", renderMainPage(appState));
}

static void handleOn()
{
    appState.ledOn = true;
    digitalWrite(LED_PIN, HIGH);
    handleRoot();
}

static void handleOff()
{
    appState.ledOn = false;
    digitalWrite(LED_PIN, LOW);
    handleRoot();
}

static void handleApi()
{
    String json = "{\"ledState\":";
    json += appState.ledOn ? "true" : "false";
    json += "}";

    server.send(200, "application/json", json);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Страница не найдена");
}

void webInit()
{
    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.on("/api", handleApi);
    server.onNotFound(handleNotFound);

    server.begin();
}

void webLoop()
{
    server.handleClient();
}