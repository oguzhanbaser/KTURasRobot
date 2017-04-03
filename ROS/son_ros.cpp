#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <string>

using namespace std;
std::string a; 
 std_msgs::String sendValue;


void chatterCallback(const std_msgs::String::ConstPtr& value)
{
  ROS_INFO("I heard: [%s]", value->data.c_str());
  sendValue.data = value->data;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber ext_sub = n.subscribe("chatter", 1000, chatterCallback);
  ros::Publisher ext_pub = n.advertise<std_msgs::String>("chatter", 1000);

  ros::Rate loop_rate(10);

   while (ros::ok())
  {
 
    //ROS_INFO("%d", msg.data);
  
    ext_pub.publish(sendValue);
    ros::spin();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}