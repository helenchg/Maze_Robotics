/***********************************************************************
* Author: Elena Chong
* Last time modified: March 19th, 2016
* Description: Maze Line Solver Vr 2.2 for the RedBot 5 IR sensors
Can perform line following. Not smooth, but acceptable. Maze 1 SOLVED
Can perform simple decision, but no left decision. Maze 2 SOLVED
Can perform simple decision, and U turn to complete the Maze!
***********************************************************************/
#include <RedBot.h>
RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);  // initialize a right sensor object on A7
RedBotSensor farR = RedBotSensor(A1);  // initialize a right sensor object on A4
RedBotSensor farL = RedBotSensor(A0);  // initialize a right sensor object on A7

#define THRESHOLD 850
#define SPEED 100  // sets the nominal speed. Set to any number from 0 - 255.
#define USPEED 110
#define UDELAY 850 // Makes it turn almost 90 degree


RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
RedBotMotors motors;
int leftSpeed = 0;   // variable used to store the leftMotor speed
int rightSpeed = 0;  // variable used to store the rightMotor speed
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D
int lCount = 0; // variables used to store the left and right encoder counts.
int rCount = 0;
int count = 0;
int leftAvg = 0;
int rightAvg = 0;
int centerAvg = 0;
int farRAvg = 0;
int farLAvg = 0;
int sdelay = 0;

void setup()
{
	Serial.begin(9600);
	delay(2000);
	Serial.println("IR Sensor Readings: ");
	delay(500);
}

void loop()
{
	// Reading data from the five IR Sensors on the redbot
	Serial.print(farL.read());
	Serial.print("\t");  // tab character
	Serial.print(left.read());
	Serial.print("\t");  // tab character
	Serial.print(center.read());
	Serial.print("\t");  // tab character
	Serial.print(right.read());
	Serial.print("\t");  // tab character
	Serial.print(farR.read());
	Serial.println();

	// Go Forward if left is negative and right is positive
	// Go Reverse if left is positive and right is negative
	// Spin CW if left is negative and right is negative
	// Spin CCW if left is positive and right is positive
	// Go Straight
	if (centerSensors() > THRESHOLD)
	{
		Serial.println("Go Straight");
		leftSpeed = -SPEED;
		rightSpeed = SPEED;
	}
	// If see right, turn slightly left
	else if (rightSensors() > THRESHOLD)
	{
		Serial.println("Turn slightly left");
		leftSpeed = -(SPEED + 40);
		rightSpeed = SPEED - 40;
	}
	// if see left, turn slightly right
	else if (leftSensors() > THRESHOLD)
	{
		Serial.println("Turn slight right");
		leftSpeed = -(SPEED - 40);
		rightSpeed = SPEED + 40;
	}
	if ((farRight() > THRESHOLD) && (rightSensors() > THRESHOLD) && (farLeft() < THRESHOLD)) {
		Serial.println("Turn RIGHT");
			leftSpeed = -SPEED;
			rightSpeed = -SPEED;

			delay(115);
	}
	if ((farLeft() > THRESHOLD) && (leftSensors() > THRESHOLD) && (centerSensors() > THRESHOLD) && (farRight() < THRESHOLD)) {
		Serial.println("deciding left");
		goForwardDecideLeft();
	}
	if ((leftSensors() > THRESHOLD) && (centerSensors() > THRESHOLD) && (rightSensors() > THRESHOLD) && (farRight() > THRESHOLD) && (farLeft() > THRESHOLD))
	{
		Serial.println("deciding right");
		goForwardDecide();
	}

	// TO-DO: Let turn performs before stop or u-turn
	if ((leftSensors() < THRESHOLD) && (centerSensors() < THRESHOLD) && (rightSensors() < THRESHOLD) && (farRight() < THRESHOLD) && (farLeft() < THRESHOLD))
	{
		//delay(100); 
		Serial.println("U-TURN");
		motors.brake();
		//driveDistance(3, -SPEED, SPEED); //drive forward 1 inches before U turn
		motors.leftMotor(100);
		motors.rightMotor(100);
		delay(UDELAY);
		//while ((leftSensors() < THRESHOLD) && (centerSensors() < THRESHOLD) && (rightSensors() < THRESHOLD) && (farRight() < THRESHOLD) && (farLeft() < THRESHOLD)) {
		//	
		//	motors.leftMotor(100);
		//	motors.rightMotor(100);
		//	if (centerSensors() > THRESHOLD && leftSensors() > THRESHOLD); {
				//motors.leftMotor(-100);
				//motors.rightMotor(-100);
		//		//delay(UDELAY*2);
		//	/*	motors.brake();
		//		delay(5000);*/
		//		break;
		//	}
		//}

	}
		motors.leftMotor(leftSpeed);
		motors.rightMotor(rightSpeed);
	delay(0);
}

// UPDATE SENSOR DATA - AVERAGE SENSOR READING.
void updateSensors() {
	int leftS;
	int centerS;
	int rightS;
	int farRS;
	int farLS;

	for (int i = 0; i < 5; i++) {
		leftS = leftS + left.read();
		rightS = rightS + right.read();
		centerS = centerS + center.read();
		farLS = farLS + farL.read();
		farRS = farRS + farR.read();
	}
	leftAvg = leftS / 5;
	rightAvg = rightS / 5;
	centerAvg = centerS / 5;
	farLAvg = farLS / 5;
	farRAvg = farRS / 5;
}
int farRight() {
	int farRS;

	for (int i = 0; i < 5; i++) {
		farRS = farRS + farR.read();
	}
	return farRAvg = farRS / 5;

}
int farLeft() {
	int farLS;

	for (int i = 0; i < 5; i++) {
		farLS = farLS + farL.read();
	}
	return farLAvg = farLS / 5;

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


/********************************************
* Go forward an inch to decide next movement.
*/
void goForwardDecide() {
	// move forward and decide based on values read.
	Serial.println("Move forward an inch");
	driveDistance(1, -SPEED, SPEED); //drive forward an inch
	delay(200); //delay
	if ((leftSensors() > THRESHOLD) && (centerSensors() > THRESHOLD) && (rightSensors() > THRESHOLD) && (farRight() > THRESHOLD) && (farLeft() > THRESHOLD))
	{
		motors.brake();
		leftSpeed = 0;
		rightSpeed = 0;
	}
	else if (farRight() < THRESHOLD) {
		//right turn
		leftSpeed = -SPEED;
		rightSpeed = -SPEED;
	}
	//else if ((right.read() < THRESHOLD) && (center.read() > THRESHOLD) && (left.read() < THRESHOLD)) {
	//	leftSpeed = -SPEED;
	//	rightSpeed = SPEED;
	//}
	////else {
	////	leftSpeed = 0;
	////	rightSpeed = 0;
	////}
	//delay(200);
	//driveDistance(1, SPEED, -SPEED);
	//delay(200);
	motors.leftMotor(leftSpeed);
	motors.rightMotor(rightSpeed);
	delay(UDELAY-100);
	motors.brake();
}
/********************************************
* Go forward an inch to decide next movement.
*/
void goForwardDecideLeft() {
	// move forward and decide based on values read.
	Serial.println("Move forward an inch");
	motors.brake();
	driveDistance(1, -SPEED, SPEED); //drive forward an inch
	//delay(200); //delay
	if (centerSensors() > THRESHOLD || rightSensors() > THRESHOLD) {
		// GO STRAIGHT
		leftSpeed = -SPEED;
		rightSpeed = SPEED;
		sdelay = 100;

	}
	else
	{

		// LEFT TURN
		leftSpeed = SPEED;
		rightSpeed = SPEED;
		sdelay = UDELAY;
	}
	motors.leftMotor(leftSpeed);
	motors.rightMotor(rightSpeed);
	delay(sdelay);
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