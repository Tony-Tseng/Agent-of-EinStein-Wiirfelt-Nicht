#include "time_controler.hpp"

double TimeControler::timer(bool reset){
	if ( reset ) {
		clock_gettime(CLOCK_REALTIME, &tick);
		return 0.0;
	}
	clock_gettime(CLOCK_REALTIME, &tock);
	return (double) ((tock.tv_sec + tock.tv_nsec*1e-9)-(double) (tick.tv_sec + tick.tv_nsec*1e-9));
}

double TimeControler::available_search_time(){
    return (time_limit - current_time)/ (step_estimation - current_step + 1);
}

void TimeControler::update_time(double interval){
    current_time += interval;
}

void TimeControler::update_step(){
    current_step += 1;
    if( current_step + 3 > step_estimation ){
        step_estimation += 7;
    }
}