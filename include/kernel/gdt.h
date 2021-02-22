/*
    gdt.h - IA32 Global Descriptor Table header file written in C++
 */

#ifndef GDT_H
#define GDT_H

#include "common/types.h"

namespace morphios {
namespace kernel {

class GlobalDescriptorTable {

    public:

    // Subclass to describe each entry in GDT
    class SegmentDescriptor {
        
        private:
            morphios::common::uint16_t limit_lo;
            morphios::common::uint16_t base_lo;
            morphios::common::uint8_t base_hi;
            morphios::common::uint8_t type;
            morphios::common::uint8_t limit_hi;
            morphios::common::uint8_t base_vhi;
        
        public:
            SegmentDescriptor(morphios::common::uint32_t base, morphios::common::uint32_t limit, morphios::common::uint8_t type);
            morphios::common::uint32_t Base();
            morphios::common::uint32_t Limit();
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
        morphios::common::uint16_t CodeSegmentSelector();
        morphios::common::uint16_t DataSegmentSelector();
};

} // namespace kernel    
} // namespace morphios

#endif // GDT_H
