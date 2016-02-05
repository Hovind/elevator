#include "finitestatemachine.h"

#include "elev.h"
#include "queue.h"
#include "timer.h"

#include "stdio.h"

static fsm_state_t fsm_state = INIT;


  ////////////////////////////////////
 /********* HELP FUNCTIONS *********/
////////////////////////////////////


void fsm_setState(int state){
	printf("State entered: %i.\n", state);
	fsm_state = state;
}


void fsm_clearFloorLights(int floor){
	if (floor != 0)
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);

	if (floor != N_FLOORS - 1)
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
	elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
}

void fsm_clearLights(){
	for (int floor = 0; floor < N_FLOORS; ++floor) {
		fsm_clearFloorLights(floor);
	}
}

void fsm_clearQueueAndLights(int floor){
	fsm_clearFloorLights(floor);
	q_removeOrder(floor);	
}


  /////////////////////////////////////////
 /************** FUNCTIONS **************/
/////////////////////////////////////////


int fsm_getState(){
	return fsm_state;
}


void fsm_evOrderButton(int floor, int button){
	q_addOrder(floor, button);
	elev_set_button_lamp(button, floor, 1);
	if (fsm_getState() == DOOR_OPEN){
		int current_floor = elev_get_floor_sensor_signal();

		if (floor == current_floor)
			timer_start();
		fsm_clearQueueAndLights(current_floor);
	}
}

void fsm_evStopPressed(){
	if (elev_get_floor_sensor_signal() == -1){
		q_setPreStopDirection();
		q_setStoppedBetweenFloors(1);
	}
	else
		elev_set_door_open_lamp(1);

	fsm_clearLights();
	q_clear();

	elev_set_motor_direction(DIRN_STOP);
	elev_set_stop_lamp(1);

	fsm_setState(EMERGENCY_STOP);
}

void fsm_evStopReleased(){
	elev_set_stop_lamp(0);
	if (elev_get_floor_sensor_signal() != -1){
		timer_start();
		fsm_setState(DOOR_OPEN);
	}
	else fsm_setState(IDLE);
}

void fsm_evTimeOut(){
	elev_set_door_open_lamp(0);
	fsm_setState(IDLE);
}

int fsm_idle_evFloorSensor(int floor){
	int open_door = q_checkIfDoorShouldOpen();
	
	if (open_door){
		elev_set_door_open_lamp(1);
		fsm_clearQueueAndLights(floor);

		timer_start();

		fsm_setState(DOOR_OPEN);
	}
	return open_door;
}

void fsm_go_evFloorSensor(int floor){
	q_setStoppedBetweenFloors(0);
	q_setPrevFloor(floor);

	elev_set_floor_indicator(floor);
	int stop = q_checkIfStopOnFloor();

	if (stop){
		elev_set_motor_direction(DIRN_STOP);
		fsm_setState(IDLE);
	}
}

void fsm_init_evExit(int floor){
	q_setPrevFloor(floor);
	elev_set_floor_indicator(floor);

	elev_set_motor_direction(DIRN_STOP);
	fsm_setState(IDLE);
}


void fsm_setDirection(){
	int direction = q_getDirection();

	if (direction != 0){
		q_setPrevDirection(direction);
		elev_set_motor_direction(direction);
		fsm_setState(GO);
	}
}