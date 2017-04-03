#include <ros/ros.h>
#include <geometry_msgs/Twist.h> 
#include <stdlib.h> 

int main(int argc, char **argv) {
     ros::init(argc, argv, "random_husky_commands");//
     ros::NodeHandle nh;//Dügümü Başlatır

     ros::Publisher ext_pub=nh.advertise<geometry_msgs::Twist>("husky/cmd_vel", 100);
     //Publisher komutu

     srand(time(0));

     
     ros::Rate rate(10);//10 Hz frekans ayarı

       while(ros::ok()) {
           geometry_msgs::Twist msg;
           msg.linear.x=4;
           msg.angular.z=6;
           ext_pub.publish(msg);
           rate.sleep();//Delay
         }
}