#include "time.h"

#define TICKS_PER_SEC 800000 //CLOCKS_PER_SEC gave inaccurate timer

static int start;


  /////////////////////////////////////////
 /************** FUNCTIONS **************/
/////////////////////////////////////////


void timer_start(){
	start = clock();
}

int timer_getTimeOut(){
	return ((clock() - start) / TICKS_PER_SEC > 3);
}