/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

ros::NodeHandle  nh;

char hello2[17] = "message received";

std_msgs::String str_msg;

ros::Publisher chatter("chatter", &str_msg);

void messageCb( const std_msgs::Empty& toggle_msg){
  str_msg.data = hello2;
  chatter.publish( &str_msg );
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );

char hello[13] = "hello world!";

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
}

void loop()
{
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(500);
}
