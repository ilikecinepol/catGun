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
const int ledPin = 12;

// Пины для сервомотора
const int servoPin = 5;
int pose;

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
  pinMode(ledPin, OUTPUT);
  

  // Подключение сервомотора
  servoZ.attach(servoPin);
  servoZ.write(zAngle);

  // Начало прослушивания порта
  server.begin();

  Serial.println("Server started");
}

// Функция для управления шаговым двигателем X
void moveX(int newPos) {
 if (newPos > pose){
   digitalWrite(xDirPin, 0); // set direction, HIGH for clockwise, LOW for anticlockwise
  }
  else{
    digitalWrite(xDirPin, 1); // set direction, HIGH for clockwise, LOW for anticlockwise
  }
 for(int x = 0; x<10; x++){ 
  digitalWrite(xStepPin,HIGH);
  delayMicroseconds(1000);
  digitalWrite(xStepPin,LOW); 
  delayMicroseconds(1000);

}
}

// Функция для управления сервомотором Z
void rotateZ(int newAngle) {
  Serial.println(newAngle);
  newAngle = map(newAngle, 0, 180, 180, 0);
  servoZ.write(newAngle);
  zAngle = newAngle;
  delay(10);
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
  } else if (request.indexOf("/ledon") != -1) {
    Serial.println("LED ON");
    digitalWrite(ledPin, HIGH);
  } else if (request.indexOf("/ledoff") != -1) {
    Serial.println("LED OFF");
    digitalWrite(ledPin, LOW);
  }

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
  //client.stop();
}
