/***********************************************************************
* Author: Elena Chong
* Last time modified: February 7th, 2016
* Description: Maze Line Solver Testing Each Piece of Algorithm
* Initial line following code from SparkFun Electronics RedBot Library Examples.
***********************************************************************/
#include <RedBot.h>
RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);  // initialize a right sensor object on A7

#define THRESHOLD 900
#define SPEED 80  // sets the nominal speed. Set to any number from 0 - 255.

RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
RedBotMotors motors;
int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI * wheelDiam; // Redbot wheel circumference = pi*D
int lCount; // variables used to store the left and right encoder counts.
// left negative, right negative == spin CW
// left positive, right positive == spin CCW
// left negative == forward
// left positive == backward
// right negative == backward
// right positive == forward

boolean decideAgain = true;
//int rightSensor;
//int leftSensor;
//int centerSensor;

enum state { // States
  start,        // 0
  straight,     // 1
  turnLeft,     // 2
  turnRight,    // 3
  uTurn,        // 4
  decideLeft,   // 5
  decideRight,  // 6
  finished      // 7
};
state nextState = start;
state prevState = start;



void setup()
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("IR Sensor Readings: ");
  delay(500);
}

void loop()
{
  Serial.print(left.read());
  Serial.print("\t");  // tab character
  Serial.print(center.read());
  Serial.print("\t");  // tab character
  Serial.print(right.read());
  Serial.println();

  //if ((right.read() > THRESHOLD) && (center.read() > THRESHOLD ) && (left.read() > THRESHOLD)) {
//    if(decideAgain){
      //goForwardDecide();  
//    }
  //}
  // BOTH NEGATIVE = CW
  // BOTH POSITIVE = CCW

  leftSpeed = SPEED;
  rightSpeed = -SPEED;
    motors.leftMotor(leftSpeed);
    motors.rightMotor(rightSpeed);
}



/********************************************
* Go forward an inch to decide next movement.
*/
void goForwardDecide() {
  // move forward and decide based on values read.
  Serial.println("Move forward an inch");
  driveDistance(1, -SPEED, SPEED); //drive forward an inch
  delay(200); //delay
  if ((right.read() < THRESHOLD) && (center.read() < THRESHOLD ) && (left.read() < THRESHOLD)) {
    leftSpeed = -(SPEED + 50);
    rightSpeed = SPEED - 50;
  }
  else if ((right.read() < THRESHOLD) && (center.read() > THRESHOLD) && (left.read() < THRESHOLD)) {
    leftSpeed = -SPEED;
    rightSpeed = SPEED;
  }
  else {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  delay(200);
  //driveDistance(2, SPEED, -SPEED);
  //delay(200);
  motors.leftMotor(leftSpeed);
  motors.rightMotor(rightSpeed);
  motors.brake();
  decideAgain = false;
}


/**
* Drive distance function from RedBot library examples
* distance in inches
* leftPower must be negative to drive forward
* rightPower must be positive to drive forward
*/
void driveDistance(float distance, int leftPower, int rightPower)
{
  long lCount = 0;
  long rCount = 0;
  float numRev;
  // debug
  Serial.print("driveDistance() ");
  Serial.print(distance);
  Serial.print(" inches at ");
  Serial.print(leftPower);
  Serial.println(" left power.");
  Serial.print(rightPower);
  Serial.println(" right power.");
  numRev = (float)distance / wheelCirc;
  Serial.println(numRev, 3);
  encoder.clearEnc(BOTH);  // clear the encoder count
  motors.leftMotor(leftPower);
  motors.rightMotor(rightPower);

  while (abs(lCount) < numRev*countsPerRev)
  {
//     while the left encoder is less than the target count -- debug print 
//     the encoder values and wait -- this is a holding loop.
    lCount = encoder.getTicks(LEFT);
    rCount = encoder.getTicks(RIGHT);
    Serial.print(lCount);
    Serial.print("\t");
    Serial.print(rCount);
    Serial.print("\t");
    Serial.println(numRev*countsPerRev);
  }
  // now apply "brakes" to stop the motors.
  motors.brake();
}
