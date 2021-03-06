/***********************************************************************
* Author: Elena Chong
* Last time modified: February 7th, 2016
* Description: Maze Line Solver Vr 1.1 for the RedBot
* Initial line following code from SparkFun Electronics RedBot Library Examples.
***********************************************************************/
#include <RedBot.h>
RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);  // initialize a right sensor object on A7

#define THRESHOLD 700
#define SPEED 65  // sets the nominal speed. Set to any number from 0 - 255.

RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
RedBotMotors motors;
int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D
int lCount; // variables used to store the left and right encoder counts.
int rCount;
int count = 0;
int leftAvg;
int rightAvg;
int centerAvg;
boolean decideAgain = true;
//int rightAvg;
//int leftAvg;
//int centerAvg;

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
	updateStates();
	motion();
	//if (nextState == straight) {
	//	// if on the line drive left and right at the same speed (left is CCW / right is CW)
	//	if (centerSensors() > THRESHOLD)
	//	{
	//		leftSpeed = -SPEED;
	//		rightSpeed = SPEED;
	//	}
	//	// if the line is under the right sensor, adjust relative speeds to turn to the right
	//	if (rightSensors() > THRESHOLD)
	//	{
	//		leftSpeed = -(SPEED + 50);
	//		rightSpeed = SPEED - 50;
	//	}
	//	// if the line is under the left sensor, adjust relative speeds to turn to the left
	//	if (leftSensors() > THRESHOLD)
	//	{
	//		leftSpeed = -(SPEED - 50);
	//		rightSpeed = SPEED + 50;
	//	}
	//	// if all sensors are on black or up in the air, stop the motors.
	//	// otherwise, run motors given the control speeds above.
	//}
	//if (nextState == turnRight) {
	//	leftSpeed = -SPEED -10;
	//	rightSpeed = -SPEED;
	//}
	//if (nextState == finished) {
	//	if ((leftSensors() > THRESHOLD) && (centerSensors() > THRESHOLD) && (rightSensors() > THRESHOLD))
	//	{
	//		//leftSpeed = -SPEED;
	//		//rightSpeed = SPEED;
	//		//goForwardDecide();
	//		motors.stop();
	//	}
	//}
	//else
	//{
	//	motors.leftMotor(leftSpeed);
	//	motors.rightMotor(rightSpeed);

	//}
	////decideAgain = true;
	delay(0);  // add a delay to decrease sensitivity.
}

//
///*************
//* Define all the states
//*/
void updateStates() {
	// input: 000
	if ((left.read() < THRESHOLD) && (center.read() < THRESHOLD) && (right.read() < THRESHOLD)) {
		prevState = nextState; //updating previous state to be what the previous action was.
		if (prevState == start) {
			nextState = start;
		}
		//else if (prevState == straight && (left.read() < THRESHOLD) && (center.read() < THRESHOLD) && (right.read() < THRESHOLD)) {
		//	nextState = uTurn;
		//}
		else if (prevState == uTurn) {
			nextState = uTurn;
		}
		else if (prevState == decideLeft) {
			nextState = turnLeft;
		}
		else if (prevState == decideRight) {
			nextState = turnRight;
		}
	}
	// input: 010
	else if ((left.read() < THRESHOLD) && (center.read() > THRESHOLD) && (right.read() < THRESHOLD)) {
		prevState = nextState;
		if (prevState == start) {
			nextState = straight;
		}
		else if (prevState == uTurn) {
			nextState = straight;
		}
		else if (prevState == turnRight) {
			nextState = straight;
		}
		else if (prevState == decideLeft) {
			nextState = straight;
		}
		else if (prevState == turnLeft) {
			nextState = straight;
		}
		else if (prevState == decideRight) {
			nextState = turnRight;
		}
		else if (prevState == finished) {
			nextState = straight;
		}
	}
	// Input: 011
	else if ((left.read() < THRESHOLD) && (center.read() > THRESHOLD) && (right.read() > THRESHOLD)) {
		prevState = nextState;
		if (prevState == straight) {
			nextState = turnRight;
		}
	}
	// Input: 110
	else if ((left.read() > THRESHOLD) && (center.read() > THRESHOLD) && (right.read() < THRESHOLD)) {
		prevState = nextState;
		if (prevState == straight) {
			nextState = decideLeft;
		}
	}
	// Input: 111
	else if ((left.read() > THRESHOLD) && (center.read() > THRESHOLD) && (right.read() > THRESHOLD)) {
		prevState = nextState;
		if (prevState == straight) {
			nextState = decideRight;
		}
		else if (prevState == decideRight) {
			nextState = finished;
		}
		else if (prevState == finished) {
			nextState = finished;
		}
	}

	// printing the values into the serial 
	delay(100);
	Serial.print("Previous State: ");
	Serial.println(prevState);
	Serial.print("Current State: ");
	Serial.println(nextState);
}

// UPDATE SENSOR DATA - AVERAGE SENSOR READINGS
//void updateSensors() {
//	int leftS;
//	int centerS;
//	int rightS;
//
//	for (int i = 0; i < 5; i++) {
//		leftS = leftS + left.read();
//		rightS = rightS + right.read();
//		centerS = centerS + center.read();
//	}
//	leftAvg = leftS / 5;
//	rightAvg = rightS / 5;
//	centerAvg = centerS / 5;
//}


/**
* Method that is called to perform all the motions based on sensor readings and the states
*/
void motion() {
	//***** going straight *****//
	if (nextState == straight) {
		if (centerSensors() > THRESHOLD)
		{
			leftSpeed = -SPEED;
			rightSpeed = SPEED;
		}
		if (rightSensors() > THRESHOLD)
		{
			leftSpeed = -(SPEED + 50);
			rightSpeed = SPEED - 50;
		}
		if (leftSensors() > THRESHOLD)
		{
			leftSpeed = -(SPEED - 50);
			rightSpeed = SPEED + 50;
		}
	}
	//** turn right **//
	else if (nextState == turnRight) {
		Serial.println("Turn Right");
		leftSpeed = -SPEED - 20;
		rightSpeed = -SPEED + 20;
	}
	//** decide right **//
	else if (nextState == decideRight) {
		goForwardDecideRight();
	}
	//** turn left **//
	else if (nextState == turnLeft) {
		Serial.println("Turn Left");
		leftSpeed = SPEED;
		rightSpeed = SPEED;
	}
	//** decide left **//
	if (nextState == decideLeft) {
		goForwardDecideLeft();
	}


	//** U TURN **//
	else if (nextState == uTurn) {
		while (center.read() < THRESHOLD) {
			leftSpeed = -SPEED;
			rightSpeed = -SPEED;
			motors.leftMotor(leftSpeed);
			motors.rightMotor(rightSpeed);
		}
	}
	else if (nextState == finished) {
		motors.brake();
	}
	//else {
	//	motors.leftMotor(leftSpeed);
	//	motors.rightMotor(rightSpeed);
	//}
	motors.leftMotor(leftSpeed);
	motors.rightMotor(rightSpeed);
}

/********************************************
* Go forward an inch to decide next movement.
*/
void goForwardDecideLeft() {
	// move forward and decide based on values read.
	Serial.println("Move forward an inch");
	driveDistance(1, -SPEED, SPEED); //drive forward an inch
	delay(200); //delay
	if ((right.read() < THRESHOLD) || (left.read() < THRESHOLD)) {
		//turn left
		leftSpeed = SPEED;
		rightSpeed = SPEED;
	}
	else {
		leftSpeed = -SPEED;
		rightSpeed = SPEED;
	}
	motors.leftMotor(leftSpeed);
	motors.rightMotor(rightSpeed);
	delay(500);
	motors.brake();
}

/********************************************
* Go forward an inch to decide next movement.
*/
void goForwardDecideRight() {
	// move forward and decide based on values read.
	Serial.println("Move forward an inch");
	driveDistance(1, -SPEED, SPEED); //drive forward an inch
	delay(200); //delay
	if ((right.read() < THRESHOLD) || (left.read() < THRESHOLD)) {
		//right turn
		leftSpeed = -SPEED;
		rightSpeed = -SPEED;
	}
	else if ((right.read() < THRESHOLD) && (center.read() > THRESHOLD) && (left.read() < THRESHOLD)) {
		leftSpeed = -SPEED;
		rightSpeed = SPEED;
	}
	delay(200);
	motors.leftMotor(leftSpeed);
	motors.rightMotor(rightSpeed);
	delay(500);
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
	numRev = (float)distance / wheelCirc;
	Serial.println(numRev, 3);
	encoder.clearEnc(BOTH);  // clear the encoder count
	motors.leftMotor(leftPower);
	motors.rightMotor(rightPower);

	while (abs(lCount) < numRev*countsPerRev)
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


int centerSensors() {
	int centerS;

	for (int i = 0; i < 5; i++) {
		centerS = centerS + center.read();
	}
	return centerAvg = centerS / 5;
}
int rightSensors() {
	int rightS;

	for (int i = 0; i < 5; i++) {
		rightS = rightS + right.read();
	}
	return rightAvg = rightS / 5;
}
int leftSensors() {
	int leftS;

	for (int i = 0; i < 5; i++) {
		leftS = leftS + left.read();
	}
	return leftAvg = leftS / 5;
}