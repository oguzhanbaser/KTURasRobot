#include <Servo.h>
#include <NewPing.h>

#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

#define MIN_SPEED 0
#define DEFAULT_ANGLE 90

#define RIGTH 0
#define MIDDLE 1
#define LEFT 2

Servo servo1;
int motorDir2 = 45;
int motorDir1 = 43;
int motorPWM = 2;

int led1 = 53, led2 = 51, led3 = 49, led4 = 47;

int sensE[3] = {31, 35, 39};
int sensT[3] = {33, 37, 41};

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(sensT[0], sensE[0], MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(sensT[1], sensE[1], MAX_DISTANCE),
  NewPing(sensT[2], sensE[2], MAX_DISTANCE)
};

unsigned long lastTime = 0, lastTimeUltrasonic = 0;
float dists[3];
int cnt = 0;

double readUltraSonic(int pIndex)
{
  return sonar[pIndex].ping_cm();
}

double readUltraSonic(int p_echo, int p_trig)
{

  digitalWrite(p_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(p_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(p_trig, LOW);
  long duration = pulseIn(p_echo, HIGH, 10000);
  double distance = (duration / 2) / 29.1;
  return distance;
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 3; i++)
  {
    pinMode(sensT[i], OUTPUT);
    pinMode(sensE[i], INPUT);
  }

  servo1.attach(3);
  pinMode(motorDir1, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  Serial.begin(9600);
  Serial3.begin(9600);
}

bool checkDistance(int pDist)
{
  if (dists[1] < pDist  && dists[1] != 0)
  {
    digitalWrite(led1, HIGH);
    return false;
  } else {
    digitalWrite(led1, LOW);
    return true;
  }
}

void driveServo(int &p_angle)
{
  if (p_angle < 60) p_angle = 60;
  if (p_angle > 120) p_angle = 120;

  servo1.write(p_angle);
  delay(10);
}

void driveMotor(int &p_speed, int &p_angle)
{
  if (p_speed >= 0)
  {
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
  } else {
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, HIGH);
    p_speed = -1 * p_speed;
  }

  if (p_speed > 200) p_speed = 200;
  if (p_speed < 0) p_speed = 0;

  analogWrite(motorPWM, p_speed);

  driveServo(p_angle);

  //  Serial.print(p_speed);
  //  Serial.print(" ");
  //  Serial.println(p_angle);
}


int limitDistance = 30;
void loop() {

  //  while(1)
  //  {
  //    int dd = 50, ds = 0;
  //    driveMotor(dd, ds);
  //  }

  // put your main code here, to run repeatedly:
  if (millis() - lastTime > 2000)
  {
    int sVal = 0;
    int aVal = 90;
    driveMotor(sVal, aVal);
  }

  if (millis() - lastTimeUltrasonic > 100)
  {
    dists[cnt] = readUltraSonic(cnt);

    Serial3.print("#|");
    Serial3.print(dists[0]);
    Serial3.print("|");
    Serial3.print(dists[1]);
    Serial3.print("|");
    Serial3.print(dists[2]);
    Serial3.println();

    cnt++;
    if (cnt > 2)
    {
      cnt = 0;
    }
    lastTimeUltrasonic = millis();
  }

  if (Serial3.available())
  {
    lastTime = millis();
    char c = Serial3.read();
    int aVal = 90;
    int sVal = 0;

    switch (c)
    {
      case '#':
        sVal = Serial3.parseInt();
        aVal = Serial3.parseInt();

        //if (checkDistance(20))
        driveMotor(sVal, aVal);
        break;
      case 'w':
        aVal = 90;
        sVal = Serial3.parseInt();

        if (!checkDistance(limitDistance))
          sVal = 0;

        driveMotor(sVal, aVal);

        break;
      case 's':
        aVal = 90;
        sVal = Serial3.parseInt();

        //if (!checkDistance(20))
        //  sVal = 0;

        driveMotor(sVal, aVal);
        break;
      case 'a':
        aVal = 65;
        sVal = Serial3.parseInt();

        if (!checkDistance(limitDistance))
          sVal = 0;

        driveMotor(sVal, aVal);
        break;
      case 'd':
        aVal = 115;
        sVal = Serial3.parseInt();

        if (!checkDistance(limitDistance))
          sVal = 0;

        driveMotor(sVal, aVal);
        break;
      case 'x':
        aVal = 90;
        sVal = 0;

        driveMotor(sVal, aVal);
        break;
    }

  } else if (!checkDistance(limitDistance))
  {
    int sVal = 0, aVal = 90;
    driveMotor(sVal, aVal);
  }


}
