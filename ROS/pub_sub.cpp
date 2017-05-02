#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <string>

using namespace std;

void chatterCallback(const std_msgs::String::ConstPtr& value)

{
  ROS_INFO("GelenVeri: [%s]", value->data.c_str());

  std::string value1 = value->data.substr(1,3);
  std::string value2 = value->data.substr(4,6);
  std::string value3 = value->data.substr(7,9);
  std::string value4 = value->data.substr(10,12);
  std::string value5 = value->data.substr(13,15);

}


int main(int argc, char **argv)
{
  
  
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber talk = n.subscribe("chatter", 1000, chatterCallback);

  ros::Publisher list = n.advertise<std_msgs::String>("list", 1000);

  ros::Rate loop_rate(10);  
   
  while (ros::ok())
  {

    std_msgs::String msg;
    std::stringstream ss;
    ss << "255"; //gönderilen pwm degeri
    msg.data = ss.str();
   // ROS_INFO("%s", msg.data.c_str()); //Gönderilen degeri terminalde görme
    list.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();

  }
  
  return 0;
}
