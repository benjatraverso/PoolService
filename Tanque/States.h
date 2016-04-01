#ifndef STATES
#define STATES

void eStateBeIdle(void);
void eStateMoveForward(void);
void eStateMoveBackwards(void);
void eStateTurnLeft(void);
void eStateTurnRight(void);
void setBoth(int MotorLeftForward, int MotorLeftBackwards, int MotorRightForward, int MotorRightBacwards);

#endif
