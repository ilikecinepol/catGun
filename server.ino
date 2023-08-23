#include <ESP8266WiFi.h>
#include <Servo.h>

// Укажите настройки Wi-Fi
const char* ssid = "Robot";
const char* password = "93qp799qP";

// Пины для шаговых двигателей
const int xStepPin = 16;
const int xDirPin = 14;
const int yStepPin = 4;
const int yDirPin = 13;

// Пины для сервомотора
const int servoPin = 5;

// Создаем объекты Servo
Servo servoZ;

// Переменные для хранения текущих состояний
int xPosition = 0;
int zAngle = 90;

// Функция для настройки шаговых двигателей (подставьте свои значения)
void setupSteppers() {
  // Настройка пинов
  pinMode(xStepPin, OUTPUT);
  pinMode(xDirPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(yDirPin, OUTPUT);
}

// Функция для управления шаговым двигателем X
void moveX(int steps, int dir) {
  digitalWrite(xDirPin, dir);
  for (int i = 0; i < steps; i++) {
    digitalWrite(xStepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(xStepPin, LOW);
    delayMicroseconds(500);
  }
}

// Функция для управления сервомотором Z
void rotateZ(int angle) {
  angle = constrain(angle, 0, 180);
  servoZ.write(angle);
}

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
  setupSteppers();

  // Подключение сервомотора
  servoZ.attach(servoPin);
  servoZ.write(zAngle);

  // Запуск сервера
  Serial.println("Server started");
}

void loop() {
  // Ждем подключения клиента
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Отправляем HTML-страницу клиенту
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
  client.println("</script>");
  client.println("</body></html>");
}
