#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

#define MIN_SPEED 0
#define DEFAULT_ANGLE 90

#define RIGTH 0
#define MIDDLE 1
#define LEFT 2

//#define MAKE_TEST

#ifndef MAKE_TEST
//use serial1 as a node client
class NewHardware : public ArduinoHardware
{
  public:
    NewHardware() : ArduinoHardware(&Serial1, 57600) {};
};

ros::NodeHandle_<NewHardware>  nh;
#else
ros::NodeHandle nh;
#endif

//function prototypes
void controlMessage( const geometry_msgs::Twist& msg) ;
double readUltraSonic(int pIndex);
double readUltraSonic(int p_echo, int p_trig);
void driveServo(int &p_angle);
void driveMotor(int &p_speed, int &p_angle);
bool checkDistance(int pDist);
void command_callback(const std_msgs::String &msg);
void debug_data(String pData);


#endif