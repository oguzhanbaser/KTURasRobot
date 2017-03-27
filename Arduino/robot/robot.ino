#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

ros::NodeHandle nh;

std_msgs::Float32MultiArray value;
ros::Publisher ext("node", &value);

float veri;
void setup()
{
  nh.initNode();
  nh.advertise(ext);
  pinMode(A0,INPUT);
  value.data_length = 1;
}
void loop()
{
  veri=analogRead(A0);
  value.data[0]=veri;
  ext.publish(& value);
  nh.spinOnce();
  delay(500);
}
