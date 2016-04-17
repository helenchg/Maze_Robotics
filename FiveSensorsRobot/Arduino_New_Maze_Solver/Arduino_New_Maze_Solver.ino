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
#define leapTime 200


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
int centerReading;
int leftReading;
int leftFarReading;
int rightReading;
int rightFarReading;
char path[11] = {'R','L','R','L','R','R','R','L','L','R','R'}; // Make an empty array with 20 allocations.
int readLength;
int pathLength = 0;
int buttonPin = 12;
boolean replayEnabled = false;
int finishCount = 0;
int led = 13;
int replaystage = 0;

void setup() {
	// put your setup code here, to run once:
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	digitalWrite(led, HIGH);
	delay(1000);
}

void loop() {
	// put your main code here, to run repeatedly:
	readSensors();
	Serial.print(leftFarReading);
	Serial.print("\t");  // tab character
	Serial.print(leftReading);
	Serial.print("\t");  // tab character
	Serial.print(centerReading);
	Serial.print("\t");  // tab character
	Serial.print(rightReading);
	Serial.print("\t");  // tab character
	Serial.print(rightFarReading);
	Serial.println();

	if (leftFarReading<THRESHOLD && rightFarReading<THRESHOLD &&
		(leftReading>THRESHOLD || rightReading>THRESHOLD || centerReading > THRESHOLD)) {
		straight();
	}
	else {
		motors.brake();
	}
	//replay();
	//else {
	//	rightHandRule();
	//}
}

void readSensors() {
	leftFarReading = farL.read();
	leftReading = left.read();
	rightReading = right.read();
	rightFarReading = farL.read();
	centerReading = center.read();
}

void rightHandRule() {
	if (leftFarReading > THRESHOLD && rightFarReading > THRESHOLD) {
		motors.leftDrive(SPEED);
		motors.rightDrive(SPEED);
		delay(leapTime);
		readSensors();

		if (leftFarReading > THRESHOLD || rightFarReading > THRESHOLD) {
			done();
		}
		if (leftFarReading < THRESHOLD && rightFarReading < THRESHOLD) {
			turnRight();
		}
	}
	if (leftFarReading > THRESHOLD) {
		leftSpeed = SPEED;
		rightSpeed = SPEED;
		motors.leftMotor(leftSpeed);
		motors.rightMotor(rightSpeed);
		delay(30);
		readSensors();
	
	if (rightFarReading > THRESHOLD) {
		delay(leapTime - 39);
		readSensors();

		if (rightFarReading > THRESHOLD && leftFarReading > THRESHOLD) {
			done();
		}
		else {
			turnRight();
			return;
		}
	}
	delay(leapTime - 30);
	readSensors();
	if (rightFarReading < THRESHOLD && centerReading < THRESHOLD && leftFarReading < THRESHOLD) {
		turnLeft();
		return;
	}

	path[pathLength] = 'S';
	// Serial.println("s");
	pathLength++;
	//Serial.print("Path length: ");
	//Serial.println(pathLength);
	if (path[pathLength - 2] == 'U') {
		//Serial.println("shortening path");
		shortPath();
	}
	straight();
}
readSensors();
if (leftFarReading<THRESHOLD && leftReading<THRESHOLD && rightReading<THRESHOLD
	&& rightFarReading<THRESHOLD && centerReading<THRESHOLD) {
	turnAround();
}

}

void straight() {
	if (centerReading > THRESHOLD)
	{
		Serial.println("Go Straight");
		motors.drive(SPEED);
	}
	// If see right, turn slightly left
	else if (rightReading > THRESHOLD)
	{
		Serial.println("Turn slightly left");
		leftSpeed = -(SPEED + 40);
		rightSpeed = SPEED - 40;
		motors.leftMotor(leftSpeed);
		motors.rightMotor(rightSpeed);
		delay(5);
		return;
	}
	// if see left, turn slightly right
	else if (leftReading > THRESHOLD)
	{
		Serial.println("Turn slight right");
		leftSpeed = -(SPEED - 40);
		rightSpeed = SPEED + 40;
		motors.leftMotor(leftSpeed);
		motors.rightMotor(rightSpeed);
		delay(5);
		return;
	}
	delay(5);
}

void shortPath() {
	if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'L') {
		pathLength = pathLength - 3;
		path[pathLength] = 'U';
	}
	if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'S') {
		pathLength = pathLength - 3;
		path[pathLength] = 'R';
	}
	if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'R') {
		pathLength = pathLength - 3;
		path[pathLength] = 'S';
	}
	if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'R') {
		pathLength = pathLength - 3;
		path[pathLength] = 'R';
	}
	if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'S') {
		pathLength = pathLength - 3;
		path[pathLength] = 'U';
	}
	if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'R') {
		pathLength = pathLength - 3;
		path[pathLength] = 'U';
	}
}

void turnRight() {
	while (rightReading > THRESHOLD || leftReading > THRESHOLD) {
		motors.leftDrive(SPEED - 20);
		motors.rightDrive(SPEED);
		delay(2);
		motors.brake();
	}
	while (leftReading < THRESHOLD) {
		motors.leftDrive(SPEED - 20);
		motors.rightDrive(SPEED);
		delay(2);
		motors.brake();
	}

	if (replaystage == 0) {
		path[pathLength] = 'R';
		Serial.println("r");
		pathLength++;
		Serial.print("Path length: ");
		Serial.println(pathLength);
		if (path[pathLength - 2] == 'U') {
			Serial.println("shortening path");
			shortPath();
		}
	}
}

void turnLeft() {
	while (leftReading > THRESHOLD) {
		motors.rightDrive(SPEED - 20);
		motors.leftDrive(SPEED);
		delay(2);
		motors.brake();
	}
	while (leftReading < THRESHOLD) {
		motors.rightDrive(SPEED - 20);
		motors.leftDrive(SPEED);
		delay(2);
		motors.brake();
	}
	while (rightReading > THRESHOLD) {
		motors.rightDrive(SPEED - 20);
		motors.leftDrive(SPEED);
		delay(2);
		motors.brake();
	}

	if (replaystage == 0) {
		path[pathLength] = 'L';
		//Serial.println("l");
		pathLength++;
		//Serial.print("Path length: ");
		//Serial.println(pathLength);
		if (path[pathLength - 2] == 'U') {
			//Serial.println("shortening path");
			shortPath();
		}
	}
}

void turnAround() {
	while (rightReading < THRESHOLD) {
		motors.pivot(SPEED);
	}
	path[pathLength] = 'U';
	pathLength++;
	straight();
	//Serial.println("u");
	//Serial.print("Path length: ");
	//Serial.println(pathLength);
}

void replay() {
	readSensors();
	if (leftFarReading < THRESHOLD && rightFarReading < THRESHOLD) {
		straight();
	}
	else {
		if (path[readLength] == 'R') {
			Serial.println('R');
			motors.drive(SPEED);
			delay(leapTime);
			turnRight();
		}
		if (path[readLength] == 'L') {
			Serial.println('L');
			motors.drive(SPEED);
			delay(leapTime);
			turnLeft();
		}
		if (path[readLength] == 'S') {
			Serial.println('S');
			motors.drive(SPEED);
			delay(leapTime);
			straight();
		}
		readLength++;
	}
	replay();
}
void done() {
	Serial.println("DONE?");
}