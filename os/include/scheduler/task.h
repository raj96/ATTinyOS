#ifndef TASK
#define TASK

#include <stdint.h>

typedef struct {
    uint8_t r[31];
    uint8_t sph, spl;
    uint8_t pch, pcl;
    uint8_t sreg;
} Task_t;

#endif