#include "elev.h"
#include "finitestatemachine.h"
#include "timer.h"

#include <stdio.h>


  ////////////////////////////////////
 /********* HELP FUNCTIONS *********/
////////////////////////////////////


void evm_getButtonSignals() {
	for (int floor = 0; floor < N_FLOORS; ++floor){
		for (int button = BUTTON_CALL_UP; button <= BUTTON_COMMAND; ++button){			//Checks only valid order buttons
			if(floor == 0 && button == BUTTON_CALL_DOWN)
				continue;
			else if (floor == N_FLOORS -1 && button == BUTTON_CALL_UP)
				continue;
			else if (elev_get_button_signal(button, floor))
				fsm_evOrderButton(floor, button);
		}
	}
}

int evm_checkEmergencyStop(){
	int stop = elev_get_stop_signal();
	if (stop){
		fsm_evStopPressed();
	}
	return stop;
}

void evm_init_checkFloorSensor(){
	int floor = elev_get_floor_sensor_signal();

	if (floor != -1)
		fsm_init_evExit(floor);
}

int evm_idle_checkFloorSensor(){
	int floor = elev_get_floor_sensor_signal();
	if (floor != -1)
		return fsm_idle_evFloorSensor(floor);
	else
		return 0;	
}

void evm_go_checkFloorSensor(){
	int floor = elev_get_floor_sensor_signal();
	if (floor != -1)
		fsm_go_evFloorSensor(floor);
}

void evm_checkStopButtonReleased(){
	if (!elev_get_stop_signal()){
		fsm_evStopReleased();
	}
}


  /////////////////////////////////////////
 /************** FUNCTIONS **************/
/////////////////////////////////////////


int evm_init(){
	// Initialize hardware
	if (!elev_init()) {
		printf("Unable to initialize elevator hardware!\n");
		return 1;
	}
	elev_set_motor_direction(DIRN_DOWN);
	evm_init_checkFloorSensor();
	return 0;
}

int evm_run(){
	while(1){
		switch(fsm_getState()){
			case INIT:
				evm_init_checkFloorSensor();
				break;

			case EMERGENCY_STOP:
				evm_checkStopButtonReleased();
				break;

			case DOOR_OPEN:
				evm_getButtonSignals();
				if (evm_checkEmergencyStop()) break;
				if (timer_getTimeOut()){
					fsm_evTimeOut();
				}
				break;

			case IDLE:
				evm_getButtonSignals();
				if (evm_checkEmergencyStop()) break;
				if (evm_idle_checkFloorSensor()) break;

				fsm_setDirection();
				break;

			case GO:
				evm_getButtonSignals();
				if (evm_checkEmergencyStop()) break;
				evm_go_checkFloorSensor();
				break;
		}
	}
	return 0;
}