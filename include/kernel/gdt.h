/*
    gdt.h - IA32 Global Descriptor Table header file written in C++
 */

#ifndef GDT_H
#define GDT_H

#include "common/types.h"

class GlobalDescriptorTable {

    public:

    // Subclass to describe each entry in GDT
    class SegmentDescriptor {
        
        private:
            uint16_t limit_lo;
            uint16_t base_lo;
            uint8_t base_hi;
            uint8_t type;
            uint8_t limit_hi;
            uint8_t base_vhi;
        
        public:
            SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
            uint32_t Base();
            uint32_t Limit();
    } __attribute__((packed));

    private:
        // GDT entries for each segment
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;
    
    public:
        // Constructor and destructor
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        // Public methods
        uint16_t CodeSegmentSelector();
        uint16_t DataSegmentSelector();
};

#endif // GDT_H
