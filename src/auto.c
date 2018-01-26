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
    setDrive(127, 3200);
    // Raise the lift while under the cone
    raiseLLift(1500);
    // Here, we will turn around. This is when it matters what side we are on.

    if (rightSide) {
        // We are on the right side of the bar

        // Spin ~30 degrees to the left
        spinLeft(900);
        // Move forward slightly
        setDrive(127, 1000);
        // Spin  ~160 degrees to complete turn
        spinLeft(3800);
    } else {
        // We are on the left side of the bar

        // Spin ~30 degrees to the right
        spinRight(900);
        // Move forward slightly
        setDrive(127, 1000);
        // Spin ~160 degrees to complete turn
        spinRight(3800);
    }

    // Drive back to start
    setDrive(127, 5000);
    // Release cone
    lowerLLift(1500);
    // Move back a little bit
    setDrive(-127, 1000);
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
    motorSet(LOWER_LIFT_L, -45);
    motorSet(LOWER_LIFT_R, -45 * -1);
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
