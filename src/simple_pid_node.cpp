#include <ros/ros.h>
#include "std_msgs/Int32.h"
#include <std_msgs/Float32.h>
#include <string.h>
#define quote(x) #x
using namespace std;
class pid_model {
    private:
    string name;
    ros::Subscriber link_in;
    ros::Subscriber link_set;
    ros::Publisher link_out;
    double kp,ki,kd,errSum=0,lastErr=0,input,setpoint,high,low;
    double compute(double Setpoint,double Input){
        double error = Setpoint - Input;
        errSum += error;
        double dErr = (error - lastErr);
        lastErr = error;
        double value = (kp*error + ki*(errSum) + kd*dErr);
        if(errSum>(high/ki){
            errSum=high/ki;
        }
        if(value>=high)
            return high;
        else if(value<=low)
            return low;
        else
            return value;
    }
    
    public:
    pid_model(double p,double i,double d,double max,double min,string topic,ros::NodeHandle n){
        kp = p;
        ki = i;
        kd = d;
        high = max;
        low = min;        
        name=topic;
        link_in = n.subscribe(topic+"_in",1,&pid_model::get_in,this);
        link_set = n.subscribe(topic+"_set",1,&pid_model::get_set,this);
        link_out = n.advertise<std_msgs::Int32>(name+"_out",1);
    }
    void get_in(const std_msgs::Int32 msg){
        input = msg.data;
        int val = compute(setpoint,input);
        std_msgs::Int32 opt;
        opt.data = val;
        link_out.publish(opt);
    }
    void get_set(const std_msgs::Int32 msg){
        setpoint = msg.data;
        int val = compute(setpoint,input);
        std_msgs::Int32 opt;
        opt.data = val;
        link_out.publish(opt);
    }
};


int main(int argc, char **argv)
{
    ros::init(argc, argv, "link1");
    double kp,ki,kd,max,min;
    string nam;
    ros::NodeHandle n("~");
    n.getParam("kp",kp);
    n.getParam("ki",ki);
    n.getParam("kd",kd);
    n.getParam("max",max);
    n.getParam("min",min);
    n.param<std::string>("nam",nam,"link1");
    string name = ros::this_node::getName();
    ROS_INFO("INITIALIZED");
    pid_model link1(kp,ki,kd,max,min,name,n);
    ROS_INFO("OBJECT CREATED");
    ROS_INFO("PID LOOP STARTED");
    while(ros::ok()){
      ros::spinOnce();  
    }
}
