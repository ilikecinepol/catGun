#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AccelStepper.h>
#include <Servo.h>

// Укажите ваши параметры Wi-Fi
const char* ssid = "Ваш_SSID";
const char* password = "Ваш_пароль";

// Укажите пины для шаговых двигателей NEMA17
#define X_STEP_PIN 5
#define X_DIR_PIN 4
#define Y_STEP_PIN 2
#define Y_DIR_PIN 0

// Укажите пин для сервомотора
#define SERVO_PIN 14

// Создаем объекты шаговых двигателей
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

// Создаем объект сервомотора
Servo servoZ;

// Создаем веб-сервер на порту 80
AsyncWebServer server(80);

// HTML-страница для управления пушкой
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Управление пушкой</title>
  <script>
    function setXPosition() {
      var xPosition = document.getElementById("xPosition").value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/setX?pos=" + xPosition, true);
      xhr.send();
    }

    function shoot() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/shoot", true);
      xhr.send();
    }

    function setZAngle() {
      var zAngle = document.getElementById("zAngle").value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/setZ?angle=" + zAngle, true);
      xhr.send();
    }
  </script>
</head>
<body>
  <h1>Управление пушкой</h1>
  <h2>Наклон пушки по оси X</h2>
  <input type="number" id="xPosition" placeholder="Позиция X">
  <button onclick="setXPosition()">Установить</button>

  <h2>Выстрел</h2>
  <button onclick="shoot()">Выстрелить</button>

  <h2>Поворот вокруг оси Z</h2>
  <input type="number" id="zAngle" placeholder="Угол Z">
  <button onclick="setZAngle()">Установить</button>
</body>
</html>
)rawliteral";

void setup() {
  // Инициализация серво
  servoZ.attach(SERVO_PIN);

  // Настройка шаговых двигателей
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Обработчики для веб-интерфейса
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/setX", HTTP_GET, [](AsyncWebServerRequest *request){
    String posX = request->arg("pos");
    int targetX = posX.toInt();
    stepperX.moveTo(targetX);
    request->send(200, "text/plain", "OK");
  });

  server.on("/shoot", HTTP_GET, [](AsyncWebServerRequest *request){
    // Логика для выстрела
    // ...
    request->send(200, "text/plain", "OK");
  });

  server.on("/setZ", HTTP_GET, [](AsyncWebServerRequest *request){
    String angle = request->arg("angle");
    int targetAngle = angle.toInt();
    servoZ.write(targetAngle);
    request->send(200, "text/plain", "OK");
  });

  // Запуск веб-сервера
  server.begin();
}

void loop() {
  // Обновление шаговых двигателей
  stepperX.run();
  stepperY.run();
}
