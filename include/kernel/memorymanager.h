/*
    memorymanagement.h - MMU class header file
*/

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "common/types.h"

namespace morphios {
namespace kernel {

// Memory chunk descriptor
struct memoryChunk {
    memoryChunk *next;
    memoryChunk *prev;
    bool isAllocated;
    common::size_t size;
};

class MemoryManager {
protected:
    memoryChunk *first;
public:
    static MemoryManager *activeMemoryManager;

    MemoryManager(common::size_t first, common::size_t size);
    ~MemoryManager();
    void *malloc(common::size_t size);
    void free(void *ptr);
};

} // namespace kernel
} // namespace morphios

// Declare "new" and "delete"
void* operator new(long unsigned size);
void* operator new[](long unsigned size);

// Placement new
void* operator new(long unsigned size, void *ptr);
void* operator new[](long unsigned size, void *ptr);

void operator delete(void *ptr);
void operator delete[](void *ptr);

#endif // MEMORYMANAGER_H
