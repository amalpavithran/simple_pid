# simple_pid
A simple PID loop for ROS with support for multiple nodes with self isolating topics depending on node name

# Parameters Available
* kp - Proportianality Constant (Datatype: /std_msgs/Int32)
* ki - Integral Constant (Datatype: /std_msgs/Int32)
* kd - Derivative Constant (Datatype: /std_msgs/Int32)
* max - Maximum output value (Datatype: /std_msgs/Int32)
* min - Minimum output value (Datatype: /std_msgs/Int32)

# Topics Subscribed
_Example node /link
* *node_name*_in (Ex: /link_in)

   Feedback input should be published to this topic
   Datatype: /std_msgs/Int32
* *node_name*_set (Ex: /link_out)

   Setpoint should be published to this node
   Datatype: /std_msgs/Int32

# Topics Published
* *node_name*_out (Ex: /link_set)

   The output of the PID loop is published to this topic
   Datatype: /std_msgs/Int32
