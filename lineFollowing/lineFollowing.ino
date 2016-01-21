/***********************************************************************
 * Author: Elena Chong
 * Last time modified: January 17th, 2016
 * Description: Maze Line Solver Vr 1.0 for the RedBot
 ***********************************************************************/

#include <RedBot.h>
// From RedBot Arduino Library
RedBotSensor IRSensor1 = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor IRSensor2 = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor IRSensor3 = RedBotSensor(A7); // initialize a sensor object on A7
RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
int buttonPin = 12;
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI * wheelDiam; // Redbot wheel circumference = pi*D
// variables used to store the left and right encoder counts.
int lCount;
int rCount;
boolean goStraight = false;
boolean goRight = false;

int threshold = 850;


void setup()
{
  //  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{
  Serial.print("IR Sensor Readings: ");
  Serial.print(IRSensor1.read());
  Serial.print("\t");  // tab character
  Serial.print(IRSensor2.read());
  Serial.print("\t");  // tab character
  Serial.print(IRSensor3.read());
  Serial.print("\t");  // tab character
  // print out to Serial Monitor the left and right encoder counts.
  Serial.println();

  delay(100);
  if (IRSensor1.read() < threshold && IRSensor2.read() > threshold && IRSensor3.read() > threshold) { // CASE 011, go forward decide straight or right turn
    Serial.println("S/R?");
    //      goForwardDecide();
    while (IRSensor3.read() > threshold) {

      motors.leftMotor(-100); //left positive is forward
      motors.rightMotor(-100); //right positive is backward
    }
    motors.brake();
  }
  if ( IRSensor1.read() < threshold && IRSensor2.read() > threshold && IRSensor3.read() < threshold) { // CASE 010, go straight
    Serial.println("S");
    while (IRSensor2.read() > threshold) {
      motors.leftMotor(-100);
      motors.rightMotor(100);
    }
    motors.brake();
  }
  if (IRSensor1.read() > threshold && IRSensor2.read() < threshold && IRSensor3.read() < threshold) { // CASE 110, go forward decide straight or left turn
    while (IRSensor1.read()) {

      motors.leftMotor(100); //left positive is forward
      motors.rightMotor(100); //right positive is backward
    }
    motors.brake();
  }
  if (IRSensor1.read() < threshold && IRSensor2.read() < threshold && IRSensor3.read() < threshold) { // CASE 000, go forward decide straight or right turn
    Serial.println("000 NO BLACK");
    
    motors.leftMotor(100); //left positive is forward
    motors.rightMotor(100); //right positive is backward
  }

}
void goForwardDecide() {
  // move forward and decide based on values read.
  Serial.println("Move forward an inch");
  driveDistance(1, -112, 100);
  if (IRSensor3.read() < threshold && IRSensor2.read() < threshold ) {
    goStraight = false;
    goRight = true;
  }
  driveDistance(1, 112, -100);
  motors.brake();
}


// Drive distance function from RedBot library examples
// distance in inches
// leftPower must be negative to drive forward
// rightPower must be positive to drive forward
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
  numRev = (float) distance / wheelCirc;
  Serial.println(numRev, 3);
  encoder.clearEnc(BOTH);  // clear the encoder count
  motors.leftMotor(leftPower);
  motors.rightMotor(rightPower);

  while (rCount < numRev * countsPerRev)
  {
    // while the left encoder is less than the target count -- debug print
    // the encoder values and wait -- this is a holding loop.
    lCount = encoder.getTicks(LEFT);
    rCount = encoder.getTicks(RIGHT);
    Serial.print(lCount);
    Serial.print("\t");
    Serial.print(rCount);
    Serial.print("\t");
    Serial.println(numRev * countsPerRev);
  }
  // now apply "brakes" to stop the motors.
  motors.brake();
}
