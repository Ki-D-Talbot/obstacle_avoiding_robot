#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>

// Define GPIO pins for motor control
#define LEFT_MOTOR_FORWARD_PIN  16 // Purple
#define LEFT_MOTOR_BACKWARD_PIN 17 // Blue
#define RIGHT_MOTOR_FORWARD_PIN 18 // Grey
#define RIGHT_MOTOR_BACKWARD_PIN 19 // White

// Define GPIO pins for ultrasonic sensor
#define TRIGGER_PIN  2
#define ECHO_PIN     3

// Define maximum distance for the ultrasonic sensor in centimeters
#define MAX_DISTANCE 200

// Define distance thresholds for obstacle detection and avoidance
#define OBSTACLE_DISTANCE_THRESHOLD 20
#define REVERSE_DURATION 250
#define TURN_DURATION 500
#define LOOK_DURATION 500

// Define servo pin
#define SERVO_PIN 10

// Define servo limits
#define SERVO_LEFT_LIMIT 45
#define SERVO_RIGHT_LIMIT 135
#define SERVO_MIDDLE 90



// Function prototypes
void move_forward();
void move_backward();
void turn_left();
void turn_right();
void stop_motors();
void center_servo();

// Create a NewPing object with the appropriate pins
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Create a Servo object
Servo servo;

void setup() {
    // Set GPIO pins as outputs
    pinMode(LEFT_MOTOR_FORWARD_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_BACKWARD_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_FORWARD_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_BACKWARD_PIN, OUTPUT);
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Attach servo to its pin
    servo.attach(SERVO_PIN);

    // Initialize serial communication
    Serial.begin(9600);

    Serial.print("Hello!\n");
}

void loop() {

    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);  
    
    // Move forward until an obstacle is detected
    move_forward();
    while (sonar.ping_cm() > OBSTACLE_DISTANCE_THRESHOLD) {
        // Continue moving forward
        delay(100);
    }

    // Obstacle detected, stop and reverse a little
    stop_motors();
    move_backward();
    delay(REVERSE_DURATION);
    stop_motors();

    // Look left
    servo.write(SERVO_LEFT_LIMIT);
    delay(LOOK_DURATION);
    int left_distance = sonar.ping_cm();
    Serial.print("Left distance: ");
    Serial.println(left_distance);

    // Look right
    servo.write(SERVO_RIGHT_LIMIT);
    delay(2 * LOOK_DURATION);
    int right_distance = sonar.ping_cm();
    Serial.print("Right distance: ");
    Serial.println(right_distance);

    // Decide which way to go
    if (left_distance > right_distance) {
        // Go left
        Serial.println("Going left");
        turn_left();
        delay(TURN_DURATION / 2);
        stop_motors();
        center_servo();
        move_forward();
        //delay(1000);
    } else {
        // Go right
        Serial.println("Going right");
        turn_right();
        delay(TURN_DURATION / 2);
        stop_motors();
        center_servo();
        move_forward();
        //delay(1000);
    }


}

// Function to move the robot forward
void move_forward() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
}

// Function to move the robot backward
void move_backward() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, HIGH);
}

// Function to turn the robot left
void turn_left() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, HIGH);

}

// Function to turn the robot right
void turn_right() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);

}

// Function to stop the motors
void stop_motors() {
    digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
}

// Function to center servo 
void center_servo(){
    servo.write(SERVO_MIDDLE);
    delay(1000);
}
