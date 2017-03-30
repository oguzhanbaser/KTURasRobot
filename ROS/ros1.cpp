//VERİ GÖNDERME

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char **argv)
{
  String ctrl[1]="~";	
  String pwm[3]="255";
  String pwm1[3]="255";
  String pwm2[3]="0";
  String pwm3[4]="0";
  String value=ctrl+"|"+pwm+"|"+pwm1+"|"+pwm2+"|"+pwm3;
  ros::init(argc, argv, "prueba");
  ros::NodeHandle n;
  ros::Publisher ext_pub("chatter", &str_msg);

  ros::Rate loop_rate(10);//Frekans ayarı 

  while (ros::ok())
  {
    std_msgs::String str_msg;
    str_msg.data = value;
    //ROS_INFO("%f", msg.data); //Terminale gönderilen data yazılır
    ext_pub.publish(str_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}