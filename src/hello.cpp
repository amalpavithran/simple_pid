#include <ros/ros.h>
#include "std_msgs/Int32.h"
#include <std_msgs/Float32.h>
#include <string.h>
#define quote(x) #x
using namespace std;
double kp,ki,kd,errSum=0,lastErr=0,input,setpoint;
class pid_model {
    private:
    string name;
    
    
    
    public:
    pid_model(double p,double i,double d,string topic){
        kp = p;
        ki = i;
        kd = d;
        name=topic;
       
    }
    
    
};
double compute(double Setpoint,double Input){
  double error = Setpoint - Input;
  errSum += error;
  double dErr = (error - lastErr);
  lastErr = error;
  double value = (kp*error + ki*(errSum) + kd*dErr);
  if(errSum>255){
      errSum=0;
  }
  if(value>=255)
      return 255;
  else if(value<=-255)
      return -255;
  else
      return value;
}
void get_in(const std_msgs::Int32 msg){
  input = msg.data;
  int val = compute(setpoint,input);
  std_msgs::Int32 opt;
  opt.data = val;
  link_out.publish(opt);
  ros::spinOnce();
}
void get_set(const std_msgs::Int32 msg){
  setpoint = msg.data;
  int val = compute(setpoint,input);
  std_msgs::Int32 opt;
  opt.data = val;
  link_out.publish(opt);
  ros::spinOnce();
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "Manipulator");
    ros::NodeHandle n;
    string name = ros::this_node::getName();
    ros::Subscriber link_in;
    ros::Subscriber link_set;
    ros::Publisher link_out;
    link_in = n.subscribe(name+"_in",1,get_in,this);
    link_set = n.subscribe(name+"_set",1,get_set,this);
    link_out = n.advertise<std_msgs::Int32>(name+"_out",1);
    while(ros::ok()){
      ros::spinOnce();  
    }
}