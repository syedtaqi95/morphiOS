/*
    multithreading.h
*/

#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include "common/common.h"
#include "kernel/gdt.h"

#define STACK_SIZE 4096 // 4 KiB

namespace morphios {
namespace kernel {

// Registers to save when switching contexts
struct CPUState {
    // General purpose registers
    common::uint32_t eax;
    common::uint32_t ebx;
    common::uint32_t ecx;
    common::uint32_t edx;
    common::uint32_t esi;
    common::uint32_t edi;
    common::uint32_t ebp;

    common::uint32_t error;

    // These registers are automatically pushed by the CPU when an interrupt occurs
    common::uint32_t eip;
    common::uint32_t cs;
    common::uint32_t eflags;
    common::uint32_t esp;
    common::uint32_t ss;
} __attribute__((packed));

class Task {
friend class TaskManager;
private:
    common::uint8_t stack[STACK_SIZE];
    CPUState *cpustate;
public:
    Task(GlobalDescriptorTable *gdt, void entryPoint());
    ~Task();
};

class TaskManager {
private:
    Task *tasks[256];
    common::int32_t numTasks;
    common::int32_t currentTask;
public:
    TaskManager();
    ~TaskManager();
    bool addTask(Task *task);
    CPUState *schedule(CPUState *cpustate);
};

}
}


#endif // MULTITHREADING_H
