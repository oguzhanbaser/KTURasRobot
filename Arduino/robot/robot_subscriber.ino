//C++ value reading SUBSCRİBER
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;

std_msgs::String str_msg;
 
//ros::Publisher ext_pub("node_arduino_pub", &str_msg);

void messageCb( const std_msgs::String& reading_value )
{
  str_msg.data = reading_value.data.toFloat(); 
  //ext_sub.publish( &str_msg );
}

ros::Subscriber<std_msgs::String> ext_sub("node_arduino_sub", &messageCb );

void setup()
{
  nh.initNode();
  //nh.advertise(ext_pub);
  nh.subscribe(sub);
}

void loop()
{
  //chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000); 
}