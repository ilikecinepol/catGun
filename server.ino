#include <ESP8266WiFi.h>

const char* ssid = "Robot";
const char* password = "93qp799qP";
const int xPosition = 0;
const int zAngle = 0;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  // Отправка HTTP-ответа
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='utf-8'>");
  client.println("<title>Управление пушкой</title>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>Панель управления пушкой</h1>");
  client.println("<h2>X Position:</h2>");
  client.println("<input type='range' min='-100' max='100' value='" + String(xPosition) + "' step='10' onchange='setXPosition(this.value)'>");
  client.println("<h2>Горизонтальное наведение:</h2>");
  client.println("<input type='range' min='0' max='180' value='" + String(zAngle) + "'  onchange='setZAngle(this.value)'>");
  client.println("<h2>Управление светодиодом</h2>");
  client.println("<button onclick='turnOnLED()'>Включить светодиод</button>");
  client.println("<button onclick='turnOffLED()'>Выключить светодиод</button>");
  client.println("<script>");
  client.println("function setXPosition(value) {");
  client.println("  var xhr = new XMLHttpRequest();");
  client.println("  xhr.open('GET', '/x?pos=' + value, true);");
  client.println("  xhr.send();");
  client.println("}");
  client.println("function setZAngle(value) {");
  client.println("  var xhr = new XMLHttpRequest();");
  client.println("  xhr.open('GET', '/z?angle=' + value, true);");
  client.println("  xhr.send();");
  client.println("}");
  client.println("function turnOnLED() {");
  client.println("  var xhr = new XMLHttpRequest();");
  client.println("  xhr.open('GET', '/ledon', true);");
  client.println("  xhr.send();");
  client.println("}");
  client.println("function turnOffLED() {");
  client.println("  var xhr = new XMLHttpRequest();");
  client.println("  xhr.open('GET', '/ledoff', true);");
  client.println("  xhr.send();");
  client.println("}");
  client.println("</script>");
  client.println("</body></html>");
  delay(10);
}
