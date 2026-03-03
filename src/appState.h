#pragma once
#include <Arduino.h>

struct AppState {
    bool ledOn;
    bool wifiConnected;
    IPAddress ip;
};

extern AppState appState;