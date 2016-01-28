/***********************************************************************
 * Author: Elena Chong
 * Last time modified: January 17th, 2016
 * Description: Maze Line Solver Vr 1.0 for the RedBot
 ***********************************************************************/

#include <RedBot.h>
// From RedBot Arduino Library
RedBotSensor left = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor right = RedBotSensor(A7); // initialize a sensor object on A7
RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10

// Initialize
int buttonPin = 12;
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D
int lCount; // variables used to store the left and right encoder counts.
int rCount;
boolean goStraight = false;
boolean goTurn = false;
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
int threshold = 900;


void setup()
{
//  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop()
{
  delay(500);
  Serial.print("IR Sensor Readings: ");
  Serial.print(left.read());
  Serial.print("\t");  // tab character
  Serial.print(center.read());
  Serial.print("\t");  // tab character
  Serial.print(right.read());
  Serial.print("\t");  // tab character
  Serial.println();

  updateStates();

}



/*************
 * Define all the states
 */
  void updateStates(){
  // input: 000
  if(left.read() < threshold && center.read() < threshold && right.read() < threshold){
    prevState = nextState; //updating previous state to be what the previous action was.
    if(prevState == start){
      nextState = start;     
    }
    else if(prevState == straight){
      nextState = uTurn;
    }
    else if(prevState == uTurn){
      nextState = uTurn;
    }
    else if(prevState == decideLeft){
      nextState = turnLeft;
    }
    else if(prevState == decideRight){
      nextState = turnRight;
    }
  }
  // input: 010
  if( left.read() < threshold && center.read() > threshold && right.read() < threshold){
    prevState = nextState;
    if(prevState == start){
      nextState = straight;
    }
    else if(prevState == uTurn){
      nextState = straight;
    }
    else if(prevState == turnRight){
      nextState = straight;
    }
    else if(prevState == decideLeft){
      nextState = straight;
    }
    else if(prevState == turnLeft){
      nextState = straight;
    }
    else if(prevState == decideRight){
      nextState = turnRight;
    }
    else if(prevState == finished){
      nextState = straight;
    }
  }
  // Input: 011
  if( left.read() < threshold && center.read() > threshold && right.read() > threshold){
    prevState = nextState;
    if(prevState == straight){
      nextState = turnRight;
    }
  }
  // Input: 110
  if(left.read() > threshold && center.read() > threshold && right.read() < threshold){
    prevState = nextState;
    if(prevState == straight){
      nextState = decideLeft;
    }
  }
  // Input: 111
  if(left.read() > threshold && center.read() > threshold && right.read() > threshold){
    prevState = nextState;
    if(prevState == straight){
      nextState = decideRight;
    }
    else if(prevState == decideRight){
      nextState = finished;
    }
    else if(prevState == finished){
      nextState = finished;
    }
  }

    // printing the values into the serial 
    delay(1000);
    Serial.print("Previous State: ");
    Serial.println(prevState);
    Serial.print("Current State: ");
    Serial.println(nextState);   
  }


/**
 * 
 */
void motion(){
     
  //***** going straight *****//
//  if(center.read() > threshold){
//    motors.leftMotor(-100);
//    motors.rightMotor(100); 
//  }
//  else if(left.read() > threshold){
//    motors.leftMotor(100);
//    motors.rightMotor(100);
//  }
//  else if(right.read() > threshold){
//    motors.leftMotor(-100);
//    motors.rightMotor(-100);    
//  }
  //***** end of going staight *****//
  
  //** turn right **//
//  if(center.read() > threshold && right.read() > threshold && left.read() < threshold && turnRight == true){
    // turn right over going straight
//    motors.leftMotor(-100);
//    motors.rightMotor(-100);   
//    turnRight = false;
//    goForwardDecide();
//    if(goTurn){
//      while(center.read() > threshold && right.read() > threshold){
//        motors.leftMotor(-100);
//        motors.rightMotor(-100); 
//      }
//    goTurn = false;
//    }
//    else if(goStraight){
//      goStraight = false;
//    motors.leftMotor(-100);
//    motors.rightMotor(100);      
//    }
//    
//  }
  //** END OF turn right **//

  //** LEFT TURN **//
//  if(center.read() > threshold && right.read() < threshold && left.read() > threshold){
    // turn right over going straight
//    motors.leftMotor(-100);
//    motors.rightMotor(-100);   
//    goForwardDecide();
//    if(goStraight){
//      goStraight = false;
//    motors.leftMotor(-100);
//    motors.rightMotor(100);      
//    }
//    else if(goTurn){
//    motors.leftMotor(100);
//    motors.rightMotor(100);
//    goTurn = false;
//    }
//  }
  //***** END OF LEFT TURN *****//
  
//  if(center.read() > threshold && right.read() > threshold && left.read() > threshold){
//    goForwardDecide();
//    if(goTurn){
//      motors.leftMotor(-100);
//      motors.rightMotor(-100);
//      goTurn = false;
//    }
//    else if(goStraight){
//      goStraight = false;
//      motors.leftMotor(-100);
//      motors.rightMotor(100);       
//    }
//  }
//  if(center.read() < threshold && left.read() < threshold && right.read() > threshold){
//    motors.brake();
//  }
//  while(center.read() > threshold && left.read() > 0 && right.read() > 0){
//    if(left.read() > threshold){
//      motors.leftMotor(-50);
//      motors.rightMotor(100);
//    }
//    if(right.read() > threshold){
//      motors.leftMotor(-100);
//      motors.rightMotor(50);
//    }
//    motors.leftMotor(-100);
//    motors.rightMotor(100);
//    
//  }
//  motors.brake();
}

/********************************************
 * Go forward an inch to decide next movement.
 */
  void goForwardDecide() {
    // move forward and decide based on values read.
    Serial.println("Move forward an inch");
  driveDistance(1, -112, 100);
  delay(200);
    if(right.read() < threshold && center.read() < threshold && left.read() < threshold) {
      goStraight = false;
      goTurn = true;
//      intersection = false;
    }
    delay(200);
  driveDistance(2, 112,-100);
  delay(200);
  motors.brake();
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
  numRev = (float) distance / wheelCirc;
  Serial.println(numRev, 3);
  encoder.clearEnc(BOTH);  // clear the encoder count
  motors.leftMotor(leftPower);
  motors.rightMotor(rightPower);

  while (abs(rCount) < numRev*countsPerRev)
  {
    // while the left encoder is less than the target count -- debug print 
    // the encoder values and wait -- this is a holding loop.
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
