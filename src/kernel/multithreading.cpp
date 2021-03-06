/*
    multithreading.cpp
*/

#include "kernel/multithreading.h"

using namespace morphios::common;
using namespace morphios::kernel;

// Task class methods

Task::Task(GlobalDescriptorTable *gdt, void entryPoint()) {
    // set cpustate address to the end of the stack
    cpustate = (CPUState *)(stack + STACK_SIZE - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;    
    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    // cpustate->esp = 0; // Not required for real mode tasks
    cpustate->eip = (uint32_t)entryPoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    // cpustate->ss = 0;
    cpustate->eflags = 0x202; // Bit 1 is always 1 in the flags register and we set the bit 9 (IF : Interrupt Enable Flag) .
}

Task::~Task() {}

// Task Manager class methods

TaskManager::TaskManager() : numTasks(0), currentTask(-1) {}

TaskManager::~TaskManager() {}

bool TaskManager::addTask(Task *task) {
    // If more than 256 tasks are running, don't add a new task
    if (numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    return true;
}

CPUState *TaskManager::schedule(CPUState *cpustate) {
    // If no tasks are running, return the current CPU state
    if (numTasks <= 0)
        return cpustate;
    
    // Save the cpustate
    if (currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;

    if (++currentTask >= numTasks)
        currentTask %= numTasks;
    
    return tasks[currentTask]->cpustate; 
}


