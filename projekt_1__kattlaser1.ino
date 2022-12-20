#include <Servo.h> // Include the Servo Library to control the servos
Servo xservo; // create servo object to control the x direction servo
Servo yservo; // create servo object to control the y direction servo

//Servo variables
int pos = 0;       // variable to store the servo position
int XServoPin = 9; // Servo pin that will control X motion
int YServoPin = 8; // Servo pin that will control Y motion

//Button variables
int buttonPin = 7; //Buttonpin is attached to pin 7
int buttonPress = 0;
bool mode = true;

//Lazer variables
int lazerPin = 6; //Laserpin is attached to pin 7
int LaserValue = 200; //Strenght of the laser

//Mode 2 variables
float min_x = 0;
float max_x = 90;
float min_y = 0;
float max_y = 150;
int min_freeze = 1500;
int max_freeze = 3000;
float minimal_movement = 3;
int random_delay;
float x_position = min_x + (max_x - min_x) / 2;
float y_position = min_y + (max_y - min_y) / 2;
float x_old_position = x_position;
float y_old_position = y_position;
float x_new_position;
float y_new_position;
float x_speed;
float y_speed;
int movement_time;


void setup() {
  pinMode(lazerPin, OUTPUT);     //The lazer will be an output
  pinMode(buttonPin, INPUT); //The buttonpress pin is an input
  digitalWrite(buttonPin, LOW); //This enables the Arduino pullup for this pin
  analogWrite(6, LaserValue);
  xservo.attach(XServoPin);  // attaches the servo on pin 9 to the servo object
  yservo.attach(YServoPin);  // attaches the servo on pin 8 to the servo object
}

//No argument and no output
void loop() {
  buttonPress = digitalRead(buttonPin);//Read the sate of the button
  if (buttonPress == HIGH) // The button press is active low , so if it is pressed we will turn the lazer on
  {
    mode = !mode;      //Activates mode one
  }
 
  if (mode == false) {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
      yservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(8);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      yservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(8);                       // waits 15 ms for the servo to reach the position
    }
  }
//Actives mode 2. Mode 2 puts the laser in a random position, at a random time and adjust its position if necessary. 
  if (mode == true) {
    movement_time = random(5, 30);
    random_delay = random(min_freeze, max_freeze);
    x_new_position = random(min_x + minimal_movement, max_x - minimal_movement);
    y_new_position = random(min_y + minimal_movement, max_y - minimal_movement);

    if ( (y_new_position > y_old_position) && (abs(y_new_position - y_old_position) < 0 )) {
      y_new_position = y_new_position + minimal_movement;
    }  else if ( (y_new_position < y_old_position) && (abs(y_new_position - y_old_position) < 0 )) {
      y_new_position = y_new_position - minimal_movement;
    }

    if ( (x_new_position > x_old_position) && (abs(x_new_position - x_old_position) < 0 )) {
      x_new_position = x_new_position + minimal_movement;
    }  else if ( (x_new_position < x_old_position) && (abs(x_new_position - x_old_position) < 0 )) {
      x_new_position = x_new_position - minimal_movement;
    }

    x_speed = (x_new_position - x_old_position) / movement_time;
    y_speed = (y_new_position - y_old_position) / movement_time;
    for (pos = 0; pos < movement_time; pos += 1) {
      x_position = x_position + x_speed;
      y_position = y_position + y_speed;
      xservo.write(x_position);
      yservo.write(y_position);
      delay(10);
    }
    x_old_position = x_new_position;
    y_old_position = y_new_position;
    delay(random_delay);

  }
}
