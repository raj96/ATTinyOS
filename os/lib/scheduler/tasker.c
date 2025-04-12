#include "scheduler/tasker.h"
#include "drivers/timer.h"
#include "drivers/gpio.h"
#include <stdlib.h>

#define MCUCR *((volatile char *)(0x35 + 0x20))

extern uint16_t stack_end;

extern void c_store();
extern void c_load();
extern void load_sp_and_pc();

const uint16_t STACK_SIZE = STACK_PER_TASK * (NUM_TASKS + 1);
const uint16_t TASK_SIZE = sizeof(Task_t);
Task_t *current_task;

int current_tid;

extern void blink_pb1();

#define CYCLE_SWITCH_AT 10

uint16_t ctxt_cycle_switch = 0;

// Context switching
void timer0_ovf_isr() {
    asm volatile("cli");
    c_store();
    ctxt_cycle_switch += 1;
    if (ctxt_cycle_switch >= CYCLE_SWITCH_AT) {
        ctxt_cycle_switch = 0;

        current_task->queued = 1;

        // Get the next queued task
        while(++current_tid < NUM_TASKS) {
            if(tasks[current_tid].queued == 1) {
                break;
            }
        }

        if(current_tid >= NUM_TASKS) {
            current_tid = 0;
            while(current_tid < NUM_TASKS) {
                if(tasks[current_tid].queued == 1) {
                    break;
                }
                current_tid++;
            }
        }

        if(current_tid >= NUM_TASKS) {
            // Something is wrong
            // Go to sleep
            MCUCR |= 1 << 5;
            asm volatile("sleep");
        }

        current_task = &(tasks[current_tid]);
        current_task->queued = 0;

        c_load();
    }
    asm volatile("reti");
}

void tasker_init() {
    current_task = NULL;

    for (int i = 0; i < NUM_TASKS; i++) {
        uint16_t sp = stack_end + ((i + 1) * STACK_PER_TASK);
        tasks[i].sph = (sp >> 8) & 0x0f;
        tasks[i].spl = sp & 0xff;
        tasks[i].sreg = 0;
        tasks[i].pch = 0x00; // Default to RESET vector
        tasks[i].pcl = 0x00; // Default to RESET vector
        tasks[i].queued = 0;
        tasks[i].paused = 0;
        tasks[i].stopped = 1;
    }

    current_tid = 0;
    ctxt_cycle_switch = 0;
}

bool tasker_add(void (*task_func)(void)) {
    uint16_t pc = (uint16_t)(task_func);

    // asm volatile("mov r0, %0": : "r"((pc >> 8) & 0x0f));
    // asm volatile("mov r1, %0": : "r"(pc & 0xff));

    // asm volatile("ldi r16, 1");

    // blink_pb1();

    for (int i = 0; i < NUM_TASKS; i++) {
        if (tasks[i].stopped == 1)
        {
            tasks[i].pch = (pc >> 8) & 0x0f;
            tasks[i].pcl = pc & 0xff;
            tasks[i].stopped = 0;
            tasks[i].queued = 1;

            return true;
        }
    }

    return false;
}

void tasker_run() {
    // Clear Interrupts
    asm volatile("cli");

    while(current_tid < NUM_TASKS) {
        if(tasks[current_tid].queued == 1) {
            current_task = &(tasks[current_tid]);
            break;
        }
    }

    if(current_task == NULL) {
        // Something wrong
        // Go to sleep
        MCUCR |= 1 << 5;
        asm volatile("sleep");
    }

    timer0_set_prescaler(CLK_DIV_1024);
    timer0_set_wgm(WGM_NORMAL);

    timer0_enable_overflow_interrupt();

    load_sp_and_pc();

    MCUCR |= 1 << 5;
    asm volatile("sleep");
}
