#ifndef __INCLUDE_FINITESTATEMACHINE_H__
#define __INCLUDE_FINITESTATEMACHINE_H__

int evm_init();
int evm_run();


typedef enum tag_fsm_state {
	INIT,
    IDLE,
    DOOR_OPEN,
    EMERGENCY_STOP,
    GO

} fsm_state_t;

int fsm_getState();

void fsm_evOrderButton(int floor, int button);
void fsm_evStopPressed();
void fsm_evStopReleased();
void fsm_evTimeOut();
int fsm_idle_evFloorSensor(int floor);
void fsm_go_evFloorSensor(int floor);
void fsm_init_evExit(int floor);

void fsm_setDirection();

#endif	//#ifndef __INCLUDE_FINITESTATEMACHINE_H__