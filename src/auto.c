    /** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

void setDrive(int speed, int duration);
void spinLeft(int duration);
void spinRight(int duration);
void lowerLLift(int duration);
void raiseLLift(int duration);

void autonomous() {

    // By default, we are on the right side of the bar
    int rightSide = 1;
    if (digitalRead(LIMIT_SWITCH) == LOW) {
        // if the limit switch is pressed, we are on the left side of the bar
        rightSide = 0;
    }

    // Move forward to get under the cone
    setDrive(127, 5700);
    /*
    // Raise the lift while under the cone
    raiseLLift(1300);
    // Spin just a tad
    if (rightSide) {
        spinRight(240);
    } else {
        spinLeft(240);
    }
    // Drive back to start
    setDrive(-127, 6300);
    // Lower the lift
    lowerLLift(880);
    // Move back away from dropped cone
    setDrive(-127, 1100);*/
}

void setDrive(int speed, int duration) {
    motorSet(L_DRIVE, speed);
    motorSet(R_DRIVE, speed);
 	delay(duration);
 	motorStop(L_DRIVE);
 	motorStop(R_DRIVE);
}
void spinLeft(int duration) {
    motorSet(L_DRIVE, -127);
    motorSet(R_DRIVE, 127);
    delay(duration);
    motorStop(L_DRIVE);
    motorStop(R_DRIVE);
}
void spinRight(int duration) {
    motorSet(L_DRIVE, 127);
    motorSet(R_DRIVE, -127);
    delay(duration);
    motorStop(L_DRIVE);
    motorStop(R_DRIVE);
}
void lowerLLift(int duration) {
    motorSet(LOWER_LIFT_L, -100);
    motorSet(LOWER_LIFT_R, -100 * -1);
    delay(duration);
    motorStop(LOWER_LIFT_L);
    motorStop(LOWER_LIFT_R);
}
void raiseLLift(int duration) {
    motorSet(LOWER_LIFT_L, 127);
    motorSet(LOWER_LIFT_R, 127 * -1);
    delay(duration);
    motorStop(LOWER_LIFT_L);
    motorStop(LOWER_LIFT_R);
}
