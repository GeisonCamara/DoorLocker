/*
 Name:		DoorLocker.ino
 Created:	10/19/2016 12:37:50 PM
 Author:	Mateus Demboski
*/
#include <Servo.h>
#include "Ultrasonic.h"

#define ULTRASONIC_TRIG_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define LED_ON_PIN 4
#define LED_OFF_PIN 5
#define SERVO_PIN 6
#define BTN_DOOR_PIN 7
#define BTN_LED_PIN 8
#define DOOR_CLOSED_DISTANCE 10 // in CM
#define SERVO_ROTATION 80

bool doorLocked = false;

bool useLedsState = true;

int buttonDoorState = LOW;

int buttonLedState = LOW;

int servoPosition = 0;

Servo servo;

Ultrasonic ultrasonic(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	
	servo.attach(SERVO_PIN);
	servo.write(servoPosition);
	
	pinMode(LED_ON_PIN, OUTPUT);
	pinMode(LED_OFF_PIN, OUTPUT);
	pinMode(BTN_DOOR_PIN, INPUT);
	pinMode(BTN_LED_PIN, INPUT);

	
	if (useLedsState) digitalWrite(LED_ON_PIN, HIGH);
	else digitalWrite(LED_ON_PIN, LOW);
	
	digitalWrite(LED_OFF_PIN, LOW);
}

// the loop function runs over and over again until power down or reset
void loop() {
	// read the state of the pushbutton value :
	buttonDoorState = digitalRead(BTN_DOOR_PIN);
	
	// check if the pushbutton is pressed.
	// if it is, the buttonState is HIGH:
	if (buttonDoorState == HIGH && doorLocked) {
		while (digitalRead(BTN_DOOR_PIN) == HIGH) delay(500);

		servoUnlockDoor();
		delay(5000);
	}

	buttonLedState = digitalRead(BTN_LED_PIN);

	if (buttonLedState == HIGH) {
		while (digitalRead(BTN_LED_PIN) == HIGH) delay(500);

		if (useLedsState) {
			digitalWrite(LED_OFF_PIN, LOW);
			digitalWrite(LED_ON_PIN, LOW);
			useLedsState = false;
		}
		else if (doorLocked) {
			digitalWrite(LED_OFF_PIN, LOW);
			digitalWrite(LED_ON_PIN, HIGH);
			useLedsState = true;
		}
		else {
			digitalWrite(LED_OFF_PIN, HIGH);
			digitalWrite(LED_ON_PIN, LOW);
			useLedsState = true;
		}
	}

	if (ultrasonic.Ranging(CM) <= DOOR_CLOSED_DISTANCE && !doorLocked)
	{
		servoLockDoor();
	}
}

void servoUnlockDoor() {
	for (servoPosition = SERVO_ROTATION; servoPosition >= 0; servoPosition--)
	{
		servo.write(servoPosition);
		delay(15);
	}

	doorLocked = false;

	if (useLedsState) {
		digitalWrite(LED_OFF_PIN, HIGH);
		digitalWrite(LED_ON_PIN, LOW);
	}
}

void servoLockDoor() {
	for (servoPosition = 0; servoPosition < SERVO_ROTATION; servoPosition++)
	{
		servo.write(servoPosition);
		delay(15);
	}

	doorLocked = true;

	if (useLedsState) {
		digitalWrite(LED_OFF_PIN, LOW);
		digitalWrite(LED_ON_PIN, HIGH);
	}
}