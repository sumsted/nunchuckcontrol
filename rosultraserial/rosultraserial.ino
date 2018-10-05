#include <ros.h>
#include <std_msgs/String.h>
#include <breadcrumbs/Ultra.h>

#define LED_PIN 13

#define US_FRONT_TRIG_PIN 11
#define US_FRONT_ECHO_PIN 12

#define US_LOW_TRIG_PIN 5
#define US_LOW_ECHO_PIN 6

#define US_LEFT_TRIG_PIN 3
#define US_LEFT_ECHO_PIN 4

#define US_RIGHT_TRIG_PIN 9
#define US_RIGHT_ECHO_PIN 10

#define US_REAR_TRIG_PIN 7
#define US_REAR_ECHO_PIN 8

#define MAX_BUFFER_SIZE 50

ros::NodeHandle  nh;
std_msgs::String ack;
breadcrumbs::Ultra ultrasonicMessage;

ros::Publisher chatter("chatter", &ack);
ros::Publisher ultrasonic("ultrasonic", &ultrasonicMessage);

char data[34] = "breadcrumbs::ultrasonic published";

bool commandProcessed = false; // check used by safety timer to tell if command issued
byte ledVal = HIGH; // safety timer flips the led on and off

long left_distance=0;
long low_distance=0;
long front_distance=0;
long right_distance=0;
long rear_distance=0;
long left_front_encoder=0;

void doStep(byte step){
    // for debugging
    // Serial.println("step: "+String(step));
    // delay(1000);
}

void writeSensorData(){
    ultrasonicMessage.front = front_distance;
    ultrasonicMessage.low = low_distance;
    ultrasonicMessage.left = left_distance;
    ultrasonicMessage.rear = rear_distance;
    ultrasonicMessage.right = right_distance;
    ultrasonic.publish(&ultrasonicMessage);
}

void writeId(){
    ack.data = data;
    chatter.publish(&ack);
}

int getDistance(int trigPin, int echoPin){
    long duration = 0;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH, 8730);
    if(duration < 0){
        return 0;
    } else if(duration >= 8730 || duration == 0){
        return 150;
    } else {
        return (duration/2) / 29.1;
    }
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(US_FRONT_TRIG_PIN, OUTPUT);
    pinMode(US_FRONT_ECHO_PIN, INPUT);
    pinMode(US_LEFT_TRIG_PIN, OUTPUT);
    pinMode(US_LEFT_ECHO_PIN, INPUT);
    pinMode(US_LOW_TRIG_PIN, OUTPUT);
    pinMode(US_LOW_ECHO_PIN, INPUT);
    pinMode(US_RIGHT_TRIG_PIN, OUTPUT);
    pinMode(US_RIGHT_ECHO_PIN, INPUT);
    pinMode(US_REAR_TRIG_PIN, OUTPUT);
    pinMode(US_REAR_ECHO_PIN, INPUT);
    nh.initNode();
    nh.advertise(chatter);
    nh.advertise(ultrasonic);
}

void loop() {
    nh.spinOnce();
    front_distance = getDistance(US_FRONT_TRIG_PIN, US_FRONT_ECHO_PIN);
    low_distance = getDistance(US_LOW_TRIG_PIN, US_LOW_ECHO_PIN);
    left_distance = getDistance(US_LEFT_TRIG_PIN, US_LEFT_ECHO_PIN);
    right_distance = getDistance(US_RIGHT_TRIG_PIN, US_RIGHT_ECHO_PIN);
    rear_distance = getDistance(US_REAR_TRIG_PIN, US_REAR_ECHO_PIN);
    ledVal = (ledVal==HIGH) ? LOW : HIGH;
    delay(100);
    digitalWrite(LED_PIN, ledVal);
    writeSensorData();
}
