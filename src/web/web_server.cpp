#include <Arduino.h>
#include <WebServer.h>
#include <LittleFS.h>

#include <appState.h>
#include <config.h>

static WebServer server(SERVER_PORT);

static void handleRoot()
{
    File file = LittleFS.open("/index.html", "r");
    if (!file)
    {
        server.send(500, "text/plain", "Error read index.html");
        return;
    }

    server.streamFile(file, "text/html");
    file.close();
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

void serveCSS() 
{
    File file = LittleFS.open("/css/style.css", "r");
    if (!file) { server.send(404, "text/plain", "Not found"); return; }
    server.streamFile(file, "text/css");
    file.close();
}

void serveJS() 
{
    File file = LittleFS.open("/js/script.js", "r");
    if (!file) { server.send(404, "text/plain", "Not found"); return; }
    server.streamFile(file, "application/javascript");
    file.close();
}

void handleSendText() 
{
    if (server.hasArg("value")) 
    {
        String text = server.arg("value");
        Serial.println(text);
        server.send(200, "text/plain", "OK");
    } 
    else 
    {
        server.send(400, "text/plain", "No value");
    }
}

void webInit()
{
   server.on("/css/style.css", serveCSS);
    server.on("/js/script.js", serveJS);

    server.on("/sendText", handleSendText);

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