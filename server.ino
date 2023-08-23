#include <SPI.h>
#include <WiFiEsp.h>
#include <WiFiEspServer.h>
#include <WiFiEspClient.h>
#include <Servo.h>
#include <AccelStepper.h>

// Укажите настройки Wi-Fi
char ssid[] = "Robot";
char pass[] = "93qp799qP";

// Укажите пины для шаговых двигателей NEMA17
#define X_STEP_PIN 16
#define X_DIR_PIN 14
#define Y_STEP_PIN 4
#define Y_DIR_PIN 13

// Укажите пин для сервомотора
#define SERVO_PIN 5

// Создаем объекты шаговых двигателей
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

// Создайте объект Servo для сервомотора
Servo servoZ;

// Создайте веб-сервер на порту 80
WiFiEspServer server(80);

void setup() {
  Serial.begin(115200);

  // Подключение к Wi-Fi
  WiFi.init(&Serial);
  if (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Ошибка подключения к Wi-Fi");
    while (true);
  }

  // Настройка шаговых двигателей
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);

  // Настройка сервомотора
  servoZ.attach(14);

  // Начальная позиция двигателей
  stepperX.setCurrentPosition(0);
  stepperY.setCurrentPosition(0);

  // Начало прослушивания порта
  server.begin();
}

void loop() {
  WiFiEspClient client = server.available();

  if (client) {
    Serial.println("Новый клиент подключен");
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (request.endsWith("\r\n\r\n")) {
          // Отправка HTML-страницы с элементами управления
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          client.println("<html>");
          client.println("<head>");
          client.println("<title>Управление пушкой</title>");
          client.println("<script>");
          client.println("function setXPosition() {");
          client.println("var xPosition = document.getElementById('xSlider').value;");
          client.println("var xhr = new XMLHttpRequest();");
          client.println("xhr.open('GET', '/setX?pos=' + xPosition, true);");
          client.println("xhr.send();");
          client.println("}");

          client.println("function shoot() {");
          client.println("var xhr = new XMLHttpRequest();");
          client.println("xhr.open('GET', '/shoot', true);");
          client.println("xhr.send();");
          client.println("}");

          client.println("function setZAngle() {");
          client.println("var zAngle = document.getElementById('zSlider').value;");
          client.println("var xhr = new XMLHttpRequest();");
          client.println("xhr.open('GET', '/setZ?angle=' + zAngle, true);");
          client.println("xhr.send();");
          client.println("}");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Управление пушкой</h1>");

          // Слайдеры и кнопки для управления
          client.println("<h2>Управление наклоном по X</h2>");
          client.println("<input type='range' min='-100' max='100' value='0' id='xSlider'>");
          client.println("<button onclick='setXPosition()'>Установить</button>");
          client.println("<br><br>");

          client.println("<h2>Выстрел</h2>");
          client.println("<button onclick='shoot()'>Выстрелить</button>");
          client.println("<br><br>");

          client.println("<h2>Управление поворотом Z</h2>");
          client.println("<input type='range' min='0' max='180' value='90' id='zSlider'>");
          client.println("<button onclick='setZAngle()'>Установить</button>");
          client.println("<br><br>");

          client.println("</body>");
          client.println("</html>");

          break;
        }
      }
    }

    delay(10);
    client.stop();
    Serial.println("Клиент отключен");
  }

  // Обновление двигателей
  stepperX.run();
  stepperY.run();
}
