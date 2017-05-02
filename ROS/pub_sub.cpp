#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <string>

using namespace std;
std::string a; 
std_msgs::String sendValue;
//char* tempValue = new char [3];
void chatterCallback(const std_msgs::String::ConstPtr& value)

{

  ROS_INFO("GelenVeri: [%s]", value->data.c_str());
  ROS_INFO("GelenVeri:EREN");
  //sendValue.data = value->data;
  //tempValue[0]=value->data.substr(0,3);
  //tempValue[1]=value->data.substr(3,6);
  //tempValue[2]=value->data.substr(6,9);

}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber talk = n.subscribe("chatter", 1000, chatterCallback);

  ros::Publisher list = n.advertise<std_msgs::String>("list", 1000);

  ros::Rate loop_rate(10);

 
 
  //ROS_INFO("%d", msg.data);
    

  while (ros::ok())
  {

    std_msgs::String msg;
    std::stringstream ss;
    ss << "111";
    msg.data = ss.str();
   // ROS_INFO("%s", msg.data.c_str());
    list.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();

  }
  return 0;
}