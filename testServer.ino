#include <ESP8266WiFi.h>
#include <Servo.h>

// Укажите настройки Wi-Fi
const char* ssid = "Ваш_SSID";
const char* password = "Ваш_пароль";

// Пины для шаговых двигателей
const int xStepPin = 5;
const int xDirPin = 4;
const int yStepPin = 2;
const int yDirPin = 0;
const int ledPin = 13;

// Пины для сервомотора
const int servoPin = 14;

// Создаем объекты Servo
Servo servoZ;

// Переменные для хранения текущих состояний
int xPosition = 0;
int zAngle = 90;

// Создаем сервер на порту 80
WiFiServer server(80);

void setup() {
  // Инициализация Serial
  Serial.begin(115200);

  // Инициализация Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Настройка шаговых двигателей
  pinMode(xStepPin, OUTPUT);
  pinMode(xDirPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(yDirPin, OUTPUT);

  // Подключение сервомотора
  servoZ.attach(servoPin);
  servoZ.write(zAngle);

  // Начало прослушивания порта
  server.begin();

  Serial.println("Server started");
}

void loop() {
  // Ожидание клиента
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Обработка запросов клиента
  String request = client.readStringUntil('\r');
  if (request.indexOf("/x") != -1) {
    int newPos = request.substring(request.indexOf("pos=") + 4).toInt();
    moveX(newPos);
  } else if (request.indexOf("/z") != -1) {
    int newAngle = request.substring(request.indexOf("angle=") + 6).toInt();
    rotateZ(newAngle);
  }
  } else if (request.indexOf("/z") != -1) {
    // ... (остальной код)
  } else if (request.indexOf("/ledon") != -1) {
    digitalWrite(ledPin, HIGH);
  } else if (request.indexOf("/ledoff") != -1) {
    digitalWrite(ledPin, LOW);
  }

  // Отправка HTTP-ответа
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head><title>Gun Control</title></head>");
  client.println("<body>");
  client.println("<h1>Gun Control</h1>");
  client.println("<h2>X Position:</h2>");
  client.println("<input type='range' min='-100' max='100' value='" + String(xPosition) + "' step='10' onchange='setXPosition(this.value)'>");
  client.println("<h2>Z Angle:</h2>");
  client.println("<input type='range' min='0' max='180' value='" + String(zAngle) + "' step='10' onchange='setZAngle(this.value)'>");
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
  client.println("<h2>Управление светодиодом</h2>");
  client.println("<button onclick='turnOnLED()'>Включить светодиод</button>");
  client.println("<button onclick='turnOffLED()'>Выключить светодиод</button>");
  client.println("<script>");
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
  client.println("</script>");
  client.println("</body></html>");

  delay(10);
  client.stop();
}

// Функция для управления шаговым двигателем X
void moveX(int newPos) {
  int stepsToMove = newPos - xPosition;
  int dir = (stepsToMove > 0) ? HIGH : LOW;
  stepsToMove = abs(stepsToMove);

  digitalWrite(xDirPin, dir);
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(xStepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(xStepPin, LOW);
    delayMicroseconds(500);
  }

  xPosition = newPos;
}

// Функция для управления сервомотором Z
void rotateZ(int newAngle) {
  newAngle = constrain(newAngle, 0, 180);
  servoZ.write(newAngle);
  zAngle = newAngle;
}