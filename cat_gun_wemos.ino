#include <Servo.h>


const int StepX = 16;
const int DirX = 14;
const int StepY = 5;
const int DirY = 12;
const int StepZ = 4;
const int DirZ = 13;
boolean clockwise = false;
int angle = 90;
Servo myservo;


void setup() {
  Serial.begin(9600);

  myservo.attach(StepY);
  //pinMode(13, OUTPUT);
  pinMode(StepX,OUTPUT);
  pinMode(DirX,OUTPUT);
  pinMode(StepY,OUTPUT);
  pinMode(DirY,OUTPUT);
  pinMode(StepZ,OUTPUT);
  pinMode( DirZ,OUTPUT);
  myservo.write(angle);

  //setup_uvn();

}
// Функция выстрела
void shot(){
  digitalWrite(DirZ, LOW); // set direction, HIGH for clockwise, LOW for anticlockwise
 
 for(int x = 0; x<1000; x++) { 
  digitalWrite(StepZ,HIGH);
  delayMicroseconds(500);
  digitalWrite(StepZ,LOW); 
  delayMicroseconds(500);
 }
delay(1000); // delay for 1 second
}
//Установка центрального положения. Не использовать, нужен концевик
void setup_uvn(){
  digitalWrite(DirX, 0); // set direction, HIGH for clockwise, LOW for anticlockwise
 
 while(digitalRead(13) == HIGH) { 
  digitalWrite(StepX,HIGH);
  delayMicroseconds(1000);
  digitalWrite(StepX,LOW); 
  delayMicroseconds(1000);
 }
delay(1000); // delay for 1 second
}
// Углы вертикальной наводки
void uvn(boolean clockwise){
  if (clockwise == true){
    digitalWrite(DirX, 0); // set direction, HIGH for clockwise, LOW for anticlockwise
  }
  else{
    digitalWrite(DirX, 1); // set direction, HIGH for clockwise, LOW for anticlockwise
  }
 for(int x = 0; x<10; x++){ 
  digitalWrite(StepX,HIGH);
  delayMicroseconds(1000);
  digitalWrite(StepX,LOW); 
  delayMicroseconds(1000);
 }
}
// Для управления по bluetooth. По аналогии можно сделать веб-интерфейс
// В приложении необходимо настроить передачу цифр 1-4 на повороты осей и 5 на выстрел

void bluetoothControl(){
   if (Serial.available())
  {
    int val = Serial.read();
    // При символе "1" включаем светодиод
    if ( val == '1')
    {
      digitalWrite(13, 1);
     clockwise=true;
     uvn(clockwise);

    }
    if ( val == '2')
    {
      digitalWrite(13, 0);
      clockwise=false;
      uvn(clockwise);

    }
    if ( val == '3')
    {
      angle = angle - 10;
      myservo.write(angle);

    }
    if ( val == '4')
    {
      angle = angle + 10;
      myservo.write(angle);

    }

    if ( val == '5')
    {
      shot();

    }
  }
}
void loop()
{

  myservo.write(100);
  //shot();
  uvn(true);
  delay(1000);
  }
  
