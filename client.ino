void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT); // Устанавливаем 13-й пин как выход для светодиода
  digitalWrite(13, 1); // Изначально выключаем светодиод
}

void loop() {
  static String receivedString = "";  // Создаем статическую строку для хранения принимаемых данных
    
  while (Serial.available()) {
    char receivedChar = Serial.read();  // Считываем символ
    if (receivedChar == '\n') {  // Если символ - перевод строки, обработать сообщение
      Serial.print("Received: ");
      Serial.println(receivedString);
      
      // Проверяем, равно ли принятое сообщение "LED ON"
      if (receivedString == "LED ON") {
        digitalWrite(13, HIGH); // Включаем светодиод
      }
      else if (receivedString == "LED OFF") {
        digitalWrite(13, LOW); // Включаем светодиод
      }
      
      receivedString = "";  // Очищаем строку для следующего сообщения
    } else {
      receivedString += receivedChar;  // Добавляем символ к строке
    }
  }
}
