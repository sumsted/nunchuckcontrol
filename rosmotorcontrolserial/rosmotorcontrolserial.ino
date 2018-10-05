#include <ros.h>
#include <std_msgs/String.h>
#include <breadcrumbs/Move.h>
#include "rovermotor.h"

#define ROS_MOVE_CHANNEL "motor_movement"

ros::NodeHandle  nh;

RoverMotor *roverMotor;

long left = 0;
long right = 0;
unsigned long lastMessage = 0;

std_msgs::String ack;
ros::Publisher chatter("chatter", &ack);

char data[35] = "breadcrumbs::Move message received";

void moveCallback( const breadcrumbs::Move &moveMessage){
    left = moveMessage.left;
    right = moveMessage.right;
    lastMessage = millis();
    chatter.publish(&ack);
}

ros::Subscriber<breadcrumbs::Move> sub(ROS_MOVE_CHANNEL, moveCallback );
// ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );


void setup()
{
    roverMotor = new RoverMotor();
    nh.initNode();
    nh.advertise(chatter);
    nh.subscribe(sub);
    ack.data = data;
}

void loop()
{
    // nh.spinOnce();
    if((millis()-lastMessage)>1000){
        Serial.println("lastTime reset");
        left = 0;
        right = 0;
    }
   roverMotor->runMotor(left, right);
   nh.spinOnce();
   delay(100);
}