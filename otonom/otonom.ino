#include <Servo.h>
#include <NewPing.h>
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

#include "configuration.h"

bool g = 1;
bool otonom = 0;

std_msgs::Float32MultiArray sensor_vals;
std_msgs::String debug_vals;
ros::Publisher sensor_vals_pub("sensors", &sensor_vals);
ros::Publisher debug_publisher("debug_pub", &debug_vals);
ros::Subscriber<std_msgs::String> command_sub("command_sub", &command_callback);
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &controlMessage );

Servo servo1;
int motorDir2 = 45;
int motorDir1 = 43;
int motorPWM = 2;
int hiz, aci;

int led1 = 53, led2 = 51, led3 = 49, led4 = 47;

int sensE[3] = {31, 35, 39};
int sensT[3] = {33, 37, 41};

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(sensT[0], sensE[0], MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(sensT[1], sensE[1], MAX_DISTANCE),
  NewPing(sensT[2], sensE[2], MAX_DISTANCE)
};

unsigned long lastTime = 0, lastTimeUltrasonic = 0, lastTime2 = 0;
float dists[3];
int cnt = 0;

void command_callback(const std_msgs::String &msg)
{
  String tData = msg.data;

  String id = tData.substring(tData.indexOf('#') + 1, tData.indexOf('|'));
  String val = tData.substring(tData.indexOf('|') + 1);

  if(id == "auto")
  {
    otonom = val.toInt();
  }
}

void debug_data(String pData)
{
  debug_vals.data = pData.c_str();
  debug_publisher.publish(&debug_vals);
}

void controlMessage( const geometry_msgs::Twist& msg) 
{

  hiz = (int)(msg.linear.x * 20);
  aci = (int)(msg.angular.z * 10) + 90;
  if(otonom) driveMotor(hiz, aci);
}

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

  nh.initNode();

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
  //  Serial.begin(9600);
  Serial3.begin(9600);

  sensor_vals.data = (float *)malloc(sizeof(float) * 4);
  sensor_vals.data_length = 4;
  nh.advertise(sensor_vals_pub);
  nh.advertise(debug_publisher);
  nh.subscribe(sub);
  nh.subscribe(command_sub);

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
  /*  if (millis() - lastTime > 2000)
    {
      int sVal = 0;
      int aVal = 90;
      //driveMotor(sVal, aVal);
    }
  */
  if (millis() - lastTimeUltrasonic > 100)
  {
    dists[cnt] = readUltraSonic(cnt);

    sensor_vals.data[cnt] = dists[cnt];

    //    Serial.print(dists[cnt]);
    //    Serial.print(", ");

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
      //      Serial.println();
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

        if (sVal < 0) g = 0;
        else g = 1;
        otonom = 0;

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
        otonom = 0;

        driveMotor(sVal, aVal);
        break;
      case '$':
        otonom = 1;
        break;
    }

  } else if (!checkDistance(limitDistance) && g)
  {
    int sVal = 0, aVal = 90;
    driveMotor(sVal, aVal);
  }


  sensor_vals_pub.publish(&sensor_vals);
  nh.spinOnce();

  //if(millis() - lastTime2 > 200){


  //lastTime2 = millis();
  //}

}
