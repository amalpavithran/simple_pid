#include <ros.h>
#include <std_msgs/Int32.h>
ros::NodeHandle  nh;
#define pwm1 11
#define pwm2 10
#define pwm3 4
#define dir1 46
#define dir2 42
#define dir3 7
void link1( const std_msgs::Int32& msg){
  digitalWrite(dir1,msg.data>0);
  analogWrite(pwm1,abs(msg.data));
}
void link2( const std_msgs::Int32& msg){
  digitalWrite(dir2,msg.data>0);
  analogWrite(pwm2,abs(msg.data));
}
void link3( const std_msgs::Int32& msg){
  digitalWrite(dir3,msg.data>0);
  analogWrite(pwm3,abs(msg.data));
}

ros::Subscriber<std_msgs::Int32> sub1("link1/out", &link1 );
ros::Subscriber<std_msgs::Int32> sub2("link2/out", &link2 );
ros::Subscriber<std_msgs::Int32> sub3("link3/out", &link3 );

std_msgs::Int32 in1,in2,in3,set1,set2,set3;
ros::Publisher pub1("link1/in",&in1);
ros::Publisher pub2("link2/in",&in2);
ros::Publisher pub3("link3/in",&in3);
ros::Publisher pub4("link1/set",&set1);
ros::Publisher pub5("link2/set",&set2);
ros::Publisher pub6("link3/set",&set3);

void setup()
{ 
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);

  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(dir3,OUTPUT);
  
  nh.initNode();

  //Subscribers
  nh.subscribe(sub1);
  nh.subscribe(sub2);
  nh.subscribe(sub3);

  //Publishers
  nh.advertise(pub1);
  nh.advertise(pub2);
  nh.advertise(pub3);
  nh.advertise(pub4);
  nh.advertise(pub5);
  nh.advertise(pub6);
}

void loop()
{ 
  //Copying Data
  in1.data = map(analogRead(A0),0,1023,0,255);
  in2.data = map(analogRead(A1),0,1023,0,255);
  in3.data = map(analogRead(A2),0,1023,0,255);
  set1.data = 100;
  set2.data = 50;
  set3.data = 120;

  //Publishing Data
  pub1.publish(&in1);
  pub2.publish(&in2);
  pub3.publish(&in3);
  pub4.publish(&set1);
  pub5.publish(&set2);
  pub6.publish(&set3);
  nh.spinOnce();
}
