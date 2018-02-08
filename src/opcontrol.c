/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

 /*
  * Controller setups:
  *
  * Main controller:
  *	Right joystick = 1 stick drive
  *  Back right buttons (6) = Lower lift
  *		Upper button = Raise
  *		Lower button = Lower
  *
  * Partner controller:
  * 	Back right buttons (6) = Upper lift
  *		Upper button = Raise
  *		Lower button = Lower
  *	Back left buttons (5) = Claw
  *		Upper button = Close claw
  *		Lower button = Open claw
  *
  */

// Main controller defines
#define MAIN_CONTROLLER 1
#define LOWER_LIFT_UP_BTN 6
#define LOWER_LIFT_DOWN_BTN 5

// Partner controller defines
#define PARTNER_CONTROLLER 2
#define UPPER_LIFT_EXT 2
#define UPPER_LIFT_BTN 7
#define CLAW_BTN 8

//Other value defines
#define JOYSTICK_TOLERANCE 17
#define POTENT_TOLERANCE 8

// The functions we will need to use for the robot
void handleDrive();
void joystickDrive();
void buttonDrive();
void handleLowerLift();
void handleUpperLift();
void handleClaw();
void handleDirections(int reversed[], int numReversed);
int toleranceCheck(int num, int tolerance);
int isWithinTolerance(int num1, int num2, int tolerance);
void debugPotents();

void operatorControl() {

    int reversedMotors[1] = {LOWER_LIFT_R};
    int numReversedMotors = sizeof(reversedMotors) / sizeof(reversedMotors[0]);

    int lPotent = -8;
    int rPotent = -8;
    while (1) {
        // if (joystickGetDigital(MAIN_CONTROLLER, 8, JOY_RIGHT)) {
            // autonomous();
        // }
        // int curLPotent = analogReadCalibrated(LEFT_POTENT);
        // int curRPotent = analogReadCalibrated(RIGHT_POTENT);
        int curLPotent = analogRead(LEFT_POTENT) - lPotentDif;
        int curRPotent = analogRead(RIGHT_POTENT) - rPotentDif;
        if (!isWithinTolerance(curLPotent, lPotent, POTENT_TOLERANCE)) {
            printf("Left: %d\n", curLPotent);
            lPotent = curLPotent;
        }
        if (!isWithinTolerance(curRPotent, rPotent, POTENT_TOLERANCE)) {
            printf("Right: %d\n", curRPotent);
            rPotent = curRPotent;
        }
        handleDrive();
        handleLowerLift();
        handleUpperLift();
        // handleClaw();

        // Reverse the motors that are designated in reversedMotors
        handleDirections(reversedMotors, numReversedMotors);
        // Delay by 20 milliseconds to wait for joystick updates
        delay(20);
    }

}

void debugPotents() {

}

// Set the drive motors to their appropriate values
void handleDrive() {
    buttonDrive();
    // joystickDrive();
}

void joystickDrive() {
    int ch2 = toleranceCheck(joystickGetAnalog(MAIN_CONTROLLER, 2), JOYSTICK_TOLERANCE);
    int ch3 = toleranceCheck(joystickGetAnalog(MAIN_CONTROLLER, 3), JOYSTICK_TOLERANCE);

    if (abs(ch2) > 0 || abs(ch3) > 0) {
        motorSet(L_DRIVE, ch3);
        motorSet(R_DRIVE, ch2);
    }
}

void buttonDrive() {
    int lSpeed;
    int rSpeed;
    if (joystickGetDigital(MAIN_CONTROLLER, 7, JOY_UP)) {
        lSpeed = 127;
        rSpeed = 127;
    } else if (joystickGetDigital(MAIN_CONTROLLER, 7, JOY_DOWN)) {
        lSpeed = -127;
        rSpeed = -127;
    } else if (joystickGetDigital(MAIN_CONTROLLER, 7, JOY_RIGHT)) {
        rSpeed = -127;
        lSpeed = 127;
    } else if (joystickGetDigital(MAIN_CONTROLLER, 7, JOY_LEFT)) {
        rSpeed = 127;
        lSpeed = -127;
    } else {
        lSpeed = 0;
        rSpeed = 0;
    }

    motorSet(L_DRIVE, lSpeed);
    motorSet(R_DRIVE, rSpeed);
}

// Set the lower lift motors to their appropriate values
void handleLowerLift() {
    if (joystickGetDigital(MAIN_CONTROLLER, 6, JOY_UP)) {
        motorSet(LOWER_LIFT_R, 127);
    } else if (joystickGetDigital(MAIN_CONTROLLER, 6, JOY_DOWN)) {
        motorSet(LOWER_LIFT_R, -64);
    } else {
        motorStop(LOWER_LIFT_R);
    }

    if (joystickGetDigital(MAIN_CONTROLLER, 5, JOY_UP)) {
        motorSet(LOWER_LIFT_L, 127);
    } else if (joystickGetDigital(MAIN_CONTROLLER, 5, JOY_DOWN)) {
        motorSet(LOWER_LIFT_L, -64);
    } else {
        motorStop(LOWER_LIFT_L);
    }
}

// Set the upper lift motors to their appropriate values
void handleUpperLift() {
    // Max height is roughly 1608
    // Smallest height is roughly -1

    // Lift
    // int lPotent = analogRead(LEFT_POTENT);
    // int rPotent = analogRead(RIGHT_POTENT);
    int liftSpeed = 0;

    if (joystickGetDigital(PARTNER_CONTROLLER, UPPER_LIFT_BTN, JOY_UP)) {
        // Move lift upwards according to curve
        liftSpeed = 127;
    } else if (joystickGetDigital(PARTNER_CONTROLLER, UPPER_LIFT_BTN, JOY_DOWN)) {
        // Move lift downwards according to curve
        liftSpeed = -20;
    }

    motorSet(UPPER_LIFT_L, liftSpeed);
    motorSet(UPPER_LIFT_R, liftSpeed);

    // Extender
    int extenderSpeed = joystickGetAnalog(PARTNER_CONTROLLER, UPPER_LIFT_EXT);
    motorSet(UPPER_EXT_L, extenderSpeed);
    motorSet(UPPER_EXT_R, extenderSpeed);

    // Claw
    int clawSpeed = 0;
    if (joystickGetDigital(PARTNER_CONTROLLER, CLAW_BTN, JOY_LEFT)) {
        clawSpeed = -67;
    } else if (joystickGetDigital(PARTNER_CONTROLLER, CLAW_BTN, JOY_RIGHT)) {
        clawSpeed = 67;
    }
    motorSet(CLAW, clawSpeed);
}

// Reverse motors that need to be reversed
void handleDirections(int reversed[], int numReversed) {
    for (int i = 0; i < numReversed; i++) {
        // Set motor i to the negative value of its current speed
        int motor = reversed[i];
        motorSet(motor, -1 * motorGet(motor));
    }
}

// Check if number is above tolerance. If it is, return that number. If not, return 0.
int toleranceCheck(int num, int tolerance) {
    if (abs(num) > tolerance) {
        return num;
    } else {
        return 0;
    }
}

int isWithinTolerance(int num1, int num2, int tolerance) {
    if ( abs (num1 - num2) <= tolerance) {
        return 1;
    } else {
        return 0;
    }
}
