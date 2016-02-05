#ifndef __INCLUDE_QUEUE_H__
#define __INCLUDE_QUEUE_H__

void q_setPreStopDirection();
void q_setStoppedBetweenFloors(int floor);
void q_setPrevDirection(int direction);
void q_setPrevFloor(int floor);

void q_addOrder(int floor, int button);
void q_removeOrder(int floor);

void q_clear();

int q_checkIfStopOnFloor();
int q_checkIfDoorShouldOpen();

int q_getDirection();

#endif	//#ifndef __INCLUDE_QUEUE_H__