#include "main.h"
int lPotent = 0;
int rPotent = 0;

void setPotents() {
    lPotent = analogReadCalibrated(LEFT_POTENT);
    rPotent = analogReadCalibrated(RIGHT_POTENT);
}
float getLeftPotent() {
    return (float)(getLeftPotentRaw()) / 2000;
}
float getRightPotent() {
    return (float)(getRightPotentRaw()) / 1720;
}

int getLeftPotentRaw() {
    if (lPotent > 0)
        return lPotent;
    return 0;
}
int getRightPotentRaw() {
    if (rPotent > 0)
        return rPotent;
    return 0;
}
