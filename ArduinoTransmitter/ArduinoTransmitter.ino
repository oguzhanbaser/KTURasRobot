<<<<<<< Updated upstream
#include <SoftwareSerial.h>

SoftwareSerial xBeeSerial(10, 11);

int potY1 = A4, potY2 = A5,
      potX2 = A6, potX1 = A7;

//swPin4 kullanılmıyor!!
int swPin1 = A0, swPin2 = A1, 
      swPin3 = A2, swPin4 = 4;

int maxSpeed = 200;

void setup() {
  // put your setup code here, to run once:
  xBeeSerial.begin(9600);
  Serial.begin(9600);

  pinMode(swPin1, INPUT_PULLUP);
  pinMode(swPin2, INPUT_PULLUP);
  pinMode(swPin3, INPUT_PULLUP);
  pinMode(swPin4, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  int valX1 = analogRead(potX1);
  int valX2 = analogRead(potX2);
  int valY1 = analogRead(potY1);
  int valY2 = analogRead(potY2);

  int swVal1 = digitalRead(swPin1);
  int swVal2 = digitalRead(swPin2);
  int swVal3 = digitalRead(swPin3);
  int swVal4 = digitalRead(swPin4);


  bool manuelControl = !swVal3;
  bool autoControl = !swVal1;
  bool standby = !swVal2;

  if(manuelControl){
    
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);

    int speedVal = map(valX1, 0, 1023, 0, maxSpeed);
    int angleVal = map(valY1, 0, 1023, 60, 120);
    int motorDir = (valY2 < 500 ? -1 : 1);

    xBeeSerial.print("#");
    xBeeSerial.print(speedVal * motorDir);
    xBeeSerial.print("|");
    xBeeSerial.println(angleVal);
  }
  else if(autoControl){
    xBeeSerial.print("$"); 
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
      
  }
  else{
    xBeeSerial.print("x");
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
  }
  
//  Serial.print(swVal1);
//  Serial.print(" ");
//  Serial.print(swVal2);
//  Serial.print(" ");
//  Serial.print(swVal3);
//  Serial.print(" ");
//  Serial.print(swVal4);
//  Serial.println(" ");

//  Serial.print(valX1);
//  Serial.print(" ");
//  Serial.print(valX2);
//  Serial.print(" ");
//  Serial.print(valY1);
//  Serial.print(" ");
//  Serial.print(valY2);
//  Serial.println(" ");

  delay(100);
}
=======
#include <SoftwareSerial.h>

SoftwareSerial xBeeSerial(10, 11);

int potY1 = A4, potY2 = A5,
      potX2 = A6, potX1 = A7;

//swPin4 kullanılmıyor!!
int swPin1 = A0, swPin2 = A1, 
      swPin3 = A2, swPin4 = 4;

int maxSpeed = 200;

void setup() {
  // put your setup code here, to run once:
  xBeeSerial.begin(9600);
  Serial.begin(9600);

  pinMode(swPin1, INPUT_PULLUP);
  pinMode(swPin2, INPUT_PULLUP);
  pinMode(swPin3, INPUT_PULLUP);
  pinMode(swPin4, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  int valX1 = analogRead(potX1);
  int valX2 = analogRead(potX2);
  int valY1 = analogRead(potY1);
  int valY2 = analogRead(potY2);

  int swVal1 = digitalRead(swPin1);
  int swVal2 = digitalRead(swPin2);
  int swVal3 = digitalRead(swPin3);
  int swVal4 = digitalRead(swPin4);


  bool manuelControl = !swVal1;

  if(manuelControl)
  {
    //manuel kontrol ledi aktif

    int speedVal = map(valX1, 0, 1023, 0, maxSpeed);
    int angleVal = map(valY1, 0, 1023, 60, 120);
    int motorDir = (valY2 < 500 ? -1 : 1);

    xBeeSerial.print("#");
    xBeeSerial.print(speedVal * motorDir);
    xBeeSerial.print("|");
    xBeeSerial.println(angleVal);
  }
  
//  Serial.print(swVal1);
//  Serial.print(" ");
//  Serial.print(swVal2);
//  Serial.print(" ");
//  Serial.print(swVal3);
//  Serial.print(" ");
//  Serial.print(swVal4);
//  Serial.println(" ");

//  Serial.print(valX1);
//  Serial.print(" ");
//  Serial.print(valX2);
//  Serial.print(" ");
//  Serial.print(valY1);
//  Serial.print(" ");
//  Serial.print(valY2);
//  Serial.println(" ");

  delay(100);
}
>>>>>>> Stashed changes
