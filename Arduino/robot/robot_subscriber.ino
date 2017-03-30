// from C++ value reading SUBSCRİBER
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;
String value[]={"value","value1","value2","value3"};

std_msgs::String str_msg;

//ros::Publisher ext_pub("node_arduino_pub", &str_msg);

void veri( const std_msgs::String& reading_value )
{
	str_msg.data = reading_value.data; 
  //ext_sub.publish( &str_msg );
	for(int i=;i<20;i+3;)
	{
			value[i/3]=str_msg.substring(i);	
	}
	for(int j=0;j<5;j++)
	{
		value[j].toFloat();
	}
}

ros::Subscriber<std_msgs::String> ext_sub("node_arduino_sub", &veri );

{
	nh.initNode();
  //nh.advertise(ext_pub);
	nh.subscribe(ext_sub);
}

void loop()
{
  //chatter.publish( &str_msg );
	nh.spinOnce();
	delay(1000); 
}