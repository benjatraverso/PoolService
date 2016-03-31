#ifndef STATES
#define STATES

void eStateBeIdle(void);
void eStateMoveForward(void);
void eStateMoveBackwards(void);
void eStateTurnLeft(void);
void eStateTurnRight(void);
void setBoth(bool MotorLeftForward, bool MotorLeftBackwards, bool MotorRightForward, bool MotorRightBacwards);

#endif
