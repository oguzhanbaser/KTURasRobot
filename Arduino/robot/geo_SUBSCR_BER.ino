#include <ros.h>
#include <geometry_msgs/Pose2D.h>

ros::NodeHandle  nh;

void poseCb( const geometry_msgs::Pose2D& pose_msg){
  float x = pose_msg.x;
  float y = pose_msg.y;
  float th = pose_msg.theta;
}

ros::Subscriber<geometry_msgs::Pose2D> ext_sub("pose", &poseCb );

void setup()
{ 
  nh.initNode();
  nh.subscribe(ext_sub);
}

void loop()
{  
  nh.spinOnce();
}
