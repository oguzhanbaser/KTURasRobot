//ANALOG VERİ OKUMA
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

ros::NodeHandle nh;

std_msgs::Float32MultiArray angulos;
ros::Publisher ext("node", &value);

float veri;
void setup()
{
  nh.initNode();
  nh.advertise(ext);
  pinMode(A0,INPUT);
}
void loop()
{
  veri=analogRead(A0);
  angulos.data[0]=veri
  nh.spinOnce();
  ext.publish(& angulos);
  delay(500);
}
