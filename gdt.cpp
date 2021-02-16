/*
    gdt.cpp - IA32 Global Descriptor Table implementation written in C++
*/

#include "gdt.h"

// GDT constructor
GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentSelector(0, 0, 0),
      unusedSegmentSelector(0, 0, 0),
      codeSegmentSelector(0, 64*1024*1024, 0x9A),
      dataSegmentSelector(0, 64*1024*1024, 0x92)
{
    uint32_t i[2];
    i[1] = (uint32_t)this;
    i[0] = sizeof(GlobalDescriptorTable) << 16;
    asm volatile("lgdt (%0)" : : "p"(((uint8_t *)i)+2));
}

// Destructor
GlobalDescriptorTable::~GlobalDescriptorTable() {}

// Offset of code segment
uint16_t GlobalDescriptorTable::CodeSegmentSelector() {
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

// Offset of data segment
uint16_t GlobalDescriptorTable::DataSegmentSelector() {
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

// Segment Descriptor constructor
GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type) {

    uint8_t *target = (uint8_t *)this;

    if (limit <= 0x10000) {
        // 16-bit address space (for backwards compatibility)
        target[6] = 0x40;
    }
    else {
        // 32-bit address space
        target[6] = 0xC0;

        // Check if last 12 bits are all 1s
        // This is done because the 32-bit limit is stored in 20-bit register space. The 12 LSBs
        // are discarded, but only if they are all 1.
        if((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
    }
    
    // Encode limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Encode the type
    target[5] = type;

}
uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() {
    
    uint8_t *target = (uint8_t *)this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}
uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() {
    
    uint8_t *target = (uint8_t *)this;

    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xFFF; // Add the 12 1-LSBs back

    return result;
}