#ifndef TASKER_H
#define TASKER_H

#include <stdint.h>
#include <stdbool.h>

#define PC_WIDTH    12  // 12-bits wide program counter to address 4096 locations

#define NUM_TASKS   2

#define STACK_PER_TASK  32

typedef struct {
    uint8_t r0;
    uint8_t r[30]; // r2-r31

    uint8_t pch, pcl;
    uint8_t sph, spl;
    uint8_t sreg;

    uint8_t queued:2;
    uint8_t paused:2;
    uint8_t stopped:2;

} __attribute__((packed)) Task_t;

Task_t tasks[NUM_TASKS];

void tasker_init();
bool tasker_add(void(*task_func)(void));
void tasker_run();


#endif