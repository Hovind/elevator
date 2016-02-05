#define N_FLOORS 4
#define N_BUTTONS 3

typedef enum direction_tag{
	DIRN_DOWN = -1,
	DIRN_STOP = 0,
	DIRN_UP = 1
} direction_t;

enum button_tag{
	BTN_UP = 0,
	BTN_DOWN = 1,
	BTN_COMMAND = 2
};

static int order_matrix[4][3];

static int prev_direction = DIRN_DOWN;
static int pre_stop_direction = DIRN_DOWN;

static int prev_floor = 0;
static int stopped_between_floors = 0;


  /////////////////////////////////////////
 /************** FUNCTIONS **************/
/////////////////////////////////////////


void q_setPreStopDirection(){
	if (!stopped_between_floors)											//Only set pre_stop_direction the first time stop is pressed between two floors
		pre_stop_direction = prev_direction;
}

void q_setStoppedBetweenFloors(int floor){
	stopped_between_floors = floor;
}

void q_setPrevDirection(int direction){
	prev_direction = direction;
}

void q_setPrevFloor(int floor){
	prev_floor = floor;
}


void q_addOrder(int floor, int button){
	order_matrix[floor][button] = 1;
}

void q_removeOrder(int floor){
	for (int button = 0; button < N_BUTTONS; ++button){
		order_matrix[floor][button] = 0;
	}
}

void q_clear(){
	for (int floor = 0; floor < N_FLOORS; ++floor){
		q_removeOrder(floor);
	}
}


int q_getDirection(){
	int direction = DIRN_STOP;												//Set tentative direction to stop, will be returned if there are no orders

	for (int floor = 0; floor < N_FLOORS; ++floor){
		for (int button = 0; button < N_BUTTONS; ++button) {
			if (order_matrix[floor][button]) {
				if (floor > prev_floor){
					if (prev_direction == DIRN_UP) return DIRN_UP;			//If order is up and direction is up return up
					else direction = DIRN_UP;								//If direction is not up, set direction tentatively to up
				}
				else if (floor < prev_floor){
					if (prev_direction == DIRN_DOWN) return DIRN_DOWN;		//If order is down and direction is down return down
					else direction = DIRN_DOWN;								//If direction is not down, set direction tentatively to down
				}
				else if (floor == prev_floor && stopped_between_floors){
					return -pre_stop_direction;								//If stopped_between_floors flag is set and order is on the perceived prev_floor,
				}															//return the direction opposite of the direction before stop was pressed
			}
		}
	}
	return direction;
} 

int q_checkIfStopOnFloor(){
	if (order_matrix[prev_floor][BTN_COMMAND]) return 1;
	else if (prev_direction == DIRN_UP && order_matrix[prev_floor][BTN_UP]) return 1;
	else if (prev_direction == DIRN_DOWN && order_matrix[prev_floor][BTN_DOWN]) return 1;
	else return (q_getDirection() != prev_direction);
}

int q_checkIfDoorShouldOpen(){
	for (int button = 0; button < N_BUTTONS; ++button){
		if (order_matrix[prev_floor][button]) return 1;       
	}
	return 0;
}
