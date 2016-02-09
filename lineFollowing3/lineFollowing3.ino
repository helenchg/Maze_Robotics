/***********************************************************************
 * Author: Elena Chong
 * Last time modified: February 7th, 2016
 * Description: Maze Line Solver Vr 1.1 for the RedBot
 ***********************************************************************/

#include <RedBot.h>
 // From RedBot Arduino Library
RedBotSensor left = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor right = RedBotSensor(A7); // initialize a sensor object on A7
RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
#define STRAIGHTSPEED 50
#define TURNSPEED 100
// Initialize
int buttonPin = 12;
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D
int lCount; // variables used to store the left and right encoder counts.
int rCount;
int rightSensor;
int leftSensor;
int centerSensor;

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
	Serial.print(leftSensor);
	Serial.print("\t");  // tab character
	Serial.print(centerSensor);
	Serial.print("\t");  // tab character
	Serial.print(rightSensor);
	Serial.print("\t");  // tab character
	Serial.println();
	updateSensors();
	updateStates();

	//***** going straight *****//
	while (nextState == straight) {
		Serial.println("Straight");
		//while (leftSensor < threshold && centerSensor > threshold && rightSensor < threshold) {
			Serial.println("Going Straight");
			if (center.read() > threshold && left.read() < threshold && right.read() < threshold) {
				delay(100);
				motors.leftMotor(-STRAIGHTSPEED);
				motors.rightMotor(STRAIGHTSPEED);
			}
			else if (center.read() < threshold && left.read() > threshold && right.read() < threshold) {
				delay(100);
				motors.leftMotor(STRAIGHTSPEED);
				motors.rightMotor(STRAIGHTSPEED);
			}
			else if (center.read() < threshold && left.read() < threshold && right.read() > threshold) {
				delay(100);
				motors.leftMotor(-STRAIGHTSPEED);
				motors.rightMotor(-STRAIGHTSPEED);
			//}
			}
			updateSensors();
			updateStates();
	}
	//** decide left **//
	if (nextState == decideLeft) {
		Serial.println("decide left");
		goForwardDecide();
		//>>>>> may need to store the nextState in order to perform the entire task correctly
	}
	//** decide right **//
	if (nextState == decideRight) {
		Serial.println("decide right");

		goForwardDecide();
		//      turnRight = false;
		//      goForwardDecide();
		//      if(goTurn){
		//        while(center.read() > threshold && right.read() > threshold){
		//          motors.leftMotor(-100);
		//          motors.rightMotor(-100); 
		//        }
		//      goTurn = false;
		//      }
		//      else if(goStraight){
		//        goStraight = false;
		//      motors.leftMotor(-100);
		//      motors.rightMotor(100);      
		//      }

		//    }
		//>>>>> may need to store the nextState in order to perform the entire task correctly
	}
	//** turn right **//
	if (nextState == turnRight) {
		Serial.println("Turn Right");
		//while (leftSensor < threshold && centerSensor > threshold && rightSensor > threshold) {
			Serial.println("Turning a Right");
			motors.leftMotor(-TURNSPEED);
			motors.rightMotor(-TURNSPEED);

		//}
			updateSensors();
			updateStates();
	}
	//** U TURN **//
	//if (nextState == uTurn) {
	//	Serial.println("U Turn");
	//	//while (rightSensor < threshold && leftSensor < threshold && centerSensor < threshold) {
	//		Serial.println("Making a U Turn");
	//		motors.leftMotor(SPEED);
	//		motors.rightMotor(SPEED);
	//	//}
	//		updateSensors();
	//		updateStates();
	//}
	//** LEFT TURN **//
	if (nextState == turnLeft) {
		Serial.println("turn left");
		//while (leftSensor > threshold && centerSensor < threshold && rightSensor < threshold) {
			Serial.println("Turning a Left");
			motors.leftMotor(TURNSPEED);
			motors.rightMotor(TURNSPEED);
		//}
			updateSensors();
			updateStates();
	}
}

/*************
 * Define all the states
 */
void updateStates() {
	// input: 000
	if (leftSensor < threshold && centerSensor < threshold && rightSensor < threshold) {
		prevState = nextState; //updating previous state to be what the previous action was.
		if (prevState == start) {
			nextState = start;
		}
		else if (prevState == straight) {
			nextState = uTurn;
		}
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
	else if (leftSensor < threshold && centerSensor > threshold && rightSensor < threshold) {
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
	else if (leftSensor < threshold && centerSensor > threshold && rightSensor > threshold) {
		prevState = nextState;
		if (prevState == straight) {
			nextState = turnRight;
		}
	}
	// Input: 110
	else if (leftSensor > threshold && centerSensor > threshold && rightSensor < threshold) {
		prevState = nextState;
		if (prevState == straight) {
			nextState = decideLeft;
		}
	}
	// Input: 111
	else if (leftSensor > threshold && centerSensor > threshold && rightSensor > threshold) {
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
void updateSensors() {
	int leftS;
	int centerS;
	int rightS;

	for (int i = 0; i < 10; i++) {
		leftS = leftS + left.read();
		rightS = rightS + right.read();
		centerS = centerS + center.read();
	}
	leftSensor = leftS / 10;
	rightSensor = rightS / 10;
	centerSensor = centerS / 10;
}
/**
 * Method that is called to perform all the motions based on sensor readings and the states
 */
void motion() {
	//***** going straight *****//
	while (nextState == straight) {
		if (center.read() > threshold) {
			motors.leftMotor(-STRAIGHTSPEED);
			motors.rightMotor(STRAIGHTSPEED);
		}
		else if (left.read() > threshold) {
			motors.leftMotor(STRAIGHTSPEED);
			motors.rightMotor(STRAIGHTSPEED);
		}
		else if (right.read() > threshold) {
			motors.leftMotor(-STRAIGHTSPEED);
			motors.rightMotor(-STRAIGHTSPEED);
		}
	}
	//** decide left **//
	while (nextState == decideLeft) {
		goForwardDecide();
		//>>>>> may need to store the nextState in order to perform the entire task correctly
	}
	//** decide right **//
	while (nextState == decideRight) {
		goForwardDecide();
		//      turnRight = false;
		//      goForwardDecide();
		//      if(goTurn){
		//        while(center.read() > threshold && right.read() > threshold){
		//          motors.leftMotor(-100);
		//          motors.rightMotor(-100); 
		//        }
		//      goTurn = false;
		//      }
		//      else if(goStraight){
		//        goStraight = false;
		//      motors.leftMotor(-100);
		//      motors.rightMotor(100);      
		//      }

		//    }
		//>>>>> may need to store the nextState in order to perform the entire task correctly
	}
	//** turn right **//
	while (nextState == turnRight) {
		Serial.println("Turn Right");
		motors.leftMotor(-TURNSPEED);
		motors.rightMotor(-TURNSPEED);
	}
	//** LEFT TURN **//
	while (nextState == turnLeft) {
		Serial.println("Turn Left");
		motors.leftMotor(TURNSPEED);
		motors.rightMotor(TURNSPEED);
	}
	//** U TURN **//
	while (nextState == uTurn) {
		Serial.println("Making a U Turn");
		motors.leftMotor(TURNSPEED);
		motors.rightMotor(TURNSPEED);
	}
}

/********************************************
 * Go forward an inch to decide next movement.
 */
void goForwardDecide() {
	// move forward and decide based on values read.
	Serial.println("Move forward an inch");
	driveDistance(1, -112, 100);
	delay(200);
	if (rightSensor < threshold && centerSensor < threshold && leftSensor < threshold) {
		goStraight = false;
		goTurn = true;
		//      intersection = false;
	}
	delay(200);
	driveDistance(2, 112, -100);
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
	numRev = (float)distance / wheelCirc;
	Serial.println(numRev, 3);
	encoder.clearEnc(BOTH);  // clear the encoder count
	motors.leftMotor(leftPower);
	motors.rightMotor(rightPower);

	while (abs(rCount) < numRev*countsPerRev)
	{
		// while the left encoder is less than the target count -- debug print 
		// the encoder values and wait -- this is a holding loop.
		//lCount = encoder.getTicks(LEFT);
		//rCount = encoder.getTicks(RIGHT);
		//Serial.print(lCount);
		//Serial.print("\t");
		//Serial.print(rCount);
		//Serial.print("\t");
		//Serial.println(numRev*countsPerRev);
	}
	// now apply "brakes" to stop the motors.
	motors.brake();
}
