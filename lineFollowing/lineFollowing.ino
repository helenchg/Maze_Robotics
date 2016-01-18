/***********************************************************************
 * Author: Elena Chong
 * Last time modified: January 17th, 2016
 * Description: Maze Line Solver Vr 1.0 for the RedBot
 ***********************************************************************/

#include <RedBot.h>
RedBotSensor IRSensor1 = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor IRSensor2 = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor IRSensor3 = RedBotSensor(A7); // initialize a sensor object on A7
RedBotMotors motors;
int midThreshold = 800;
int rightThreshold = 800;
int leftThreshold = 800;
boolean midGo;
boolean rightGo;
boolean leftGo;

void setup()
{
  Serial.begin(9600);
  Serial.println("Welcome to experiment 6!");
  Serial.println("------------------------");
}

void loop()
{

  // keep updating the status of the sensors.
  // 1 mean "sensor sees black"
  // 0 mean "sensor sees white"
  if (IRSensor1.read() > leftThreshold) {
    leftGo = 1;
  }
  if (IRSensor2.read() > midThreshold) {
    midGo = 1;
  }
  if (IRSensor3.read() > rightThreshold) {
    rightGo = 1;
  }
  midGo = 0;
  leftGo = 0;
  rightGo = 0;
  Serial.print("IR Sensor Readings: ");
  Serial.print(IRSensor1.read());
  Serial.print("\t");  // tab character
  Serial.print(IRSensor2.read());
  Serial.print("\t");  // tab character
  Serial.print(IRSensor3.read());
  Serial.println();
  delay(100);
  // If all three sensors read white, then stop moving.
//  if (midGo == 0 && leftGo == 0 && rightGo == 0) {
//    motors.stop();
//  }

  if (rightGo) {
    motors.leftMotor(-155);
    motors.rightMotor(-155);
  }  
  if (midGo) {
    motors.drive(200);
  }
//  if(leftGo){
//    motors.leftMotor(155);
//    motors.rightMotor(155);    
//  }
//  delay(200);
//  motors.stop();
  //    motors.leftMotor(-155);
  //    motors.rightMotor(-155);
  //  else if (IRSensor2.read() >800){
  //  motors.drive(100);
  //  }
  //  else if(IRSensor2.read() > 500){
  //    motors.rightMotor(100);
  //  }
}

