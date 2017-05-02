
#include <ros.h>
#include <std_msgs/String.h>
ros::NodeHandle  nh;

std_msgs::String str_msg;

String inString;
String sendSerial;
String sendSens;
char sendValue[10];
char sendSensor[50];
String dizi[] = {"value1", "value2", "value3", "value4", "value5", "value6"};
int analog[] = {A0, A1, A2, A3, A4, A5};
void Veri( const std_msgs::String& cmd_msg) {
  sendSerial = cmd_msg.data;
  inString = cmd_msg.data;
  int value = inString.toInt();
  analogWrite(11, value);

}

ros::Subscriber<std_msgs::String> list("list", Veri);
ros::Publisher talk("chatter", &str_msg);

void setup() {
  pinMode(11, OUTPUT);
  for (int i = 0; i < 5; i++)
  {
    pinMode(analog[i], INPUT);
  }

  nh.initNode();
  nh.subscribe(list);
  nh.advertise(talk);
}

void loop() {
  sendSerial.toCharArray(sendValue, 10);
  for (int i = 0; i < 6; i++)
  {
    dizi[i] = String(255);
  }
  sendSens = "#" + dizi[0] + "|" + dizi[1] + "|" + dizi[2] + "|" + dizi[3] + "|" + dizi[4] + "|" + dizi[5] ;
  sendSens.toCharArray(sendSensor, 50);
  str_msg.data = sendValue;
  str_msg.data = sendSensor;
  talk.publish( &str_msg );
  nh.spinOnce();
  delay(100);
}
