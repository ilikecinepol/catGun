void setup() {
  Serial.begin(115200);
}

void loop() {
  static String receivedString = "";  // Создаем статическую строку для хранения принимаемых данных
    
  while (Serial.available()) {
    char receivedChar = Serial.read();  // Считываем символ
    if (receivedChar == '\n') {  // Если символ - перевод строки, обработать сообщение
      Serial.print("Received: ");
      Serial.println(receivedString);
      receivedString = "";  // Очистить строку для следующего сообщения
    } else {
      receivedString += receivedChar;  // Добавить символ к строке
    }
  }
}
