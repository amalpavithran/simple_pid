#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <string.h>
#define quote(x) #x
using namespace std;

//PID Class
class pid_model {
    private:

    //Publishers & Subscribers
    ros::Subscriber link_in;   //Input Subscriber
    ros::Subscriber link_set;  //Setpoint Subscriber
    ros::Publisher link_out;   //Output Publisher
    
    //Private Variables
    string name;
    double kp,ki,kd; //PID constants
    double errSum=0,lastErr=0,input,setpoint,high,low;

    //PID Compute loop
    double compute(double Setpoint,double Input){
        double error = Setpoint - Input;
        errSum += error;
        double dErr = (error - lastErr);
        lastErr = error;
        double value = (kp*error + ki*(errSum) + kd*dErr);
	
	//Intergral Limiter
        if(errSum>(high/ki)){
            errSum=high/ki;
        }

	//Output Limiter
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

	//Creates or Subscribes to topics
        link_in = n.subscribe(topic+"_in",1,&pid_model::get_in,this);
        link_set = n.subscribe(topic+"_set",1,&pid_model::get_set,this);
        link_out = n.advertise<std_msgs::Int32>(name+"_out",1);
    }

    //Input Callback Function
    void get_in(const std_msgs::Int32 msg){
        input = msg.data;
        int val = compute(setpoint,input);
        std_msgs::Int32 opt;
        opt.data = val;
        link_out.publish(opt);
    }

    //Setpoint Callback Function
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
    
    //Create ROS node instance
    ros::NodeHandle n("~");

    //Get parameters from launch file
    n.getParam("kp",kp);
    n.getParam("ki",ki);
    n.getParam("kd",kd);
    n.getParam("max",max);
    n.getParam("min",min);

    //Gets name of the node to use for isolating topics
    string name = ros::this_node::getName();
    ROS_INFO("INITIALIZED");
    pid_model link1(kp,ki,kd,max,min,name,n);
    ROS_INFO("OBJECT CREATED");
    ROS_INFO("PID LOOP STARTED");

    //Keepalive
    while(ros::ok()){
      ros::spinOnce();  
    }
}
