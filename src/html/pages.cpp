#include "pages.h"
#include <./app_state.h>

String renderMainPage(AppState appState) {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>ESP32-C3</title>
</head>
<body>
<h3>Статус LED: )rawliteral";

    html += appState.ledOn ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН";

    html += R"rawliteral(</h3>
<a href="/on">ВКЛЮЧИТЬ</a>
<a href="/off">ВЫКЛЮЧИТЬ</a>
</body>
</html>
)rawliteral";

    return html;
}