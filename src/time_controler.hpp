#pragma once
#include <time.h>

class TimeControler {
public:
    struct timespec tick, tock;
    double time_limit = 238.0;
    double current_time = 0.0;
    int current_step = 0;
    int step_estimation = 8;

    double timer(bool reset);
    double available_search_time();
    void update_time(double interval);
    void update_step();
};