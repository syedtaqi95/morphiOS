/*
    interrupts.h - Header file for x86 interrupts 
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class interruptsHandler {

    protected:
        
        // IDT gate (entry) definition
        struct Gate {
            uint16_t offsetLo;
            uint16_t selector;
            uint8_t zero;
            uint8_t typeAttr;
            uint16_t offsetHi;
        } __attribute__((packed));

        static Gate interruptDesciptorTable[256];

        struct interruptDesciptorTablePointer {
            uint16_t size;
            uint32_t base;
        } __attribute__((packed));

        // Create interrupt handlers
        static void HandlerIRQ0x00();
        static void HandlerIRQ0x01();
        static void HandlerIRQ0x02();
        static void HandlerIRQ0x03();
        static void HandlerIRQ0x04();
        static void HandlerIRQ0x05();
        static void HandlerIRQ0x06();
        static void HandlerIRQ0x07();
        static void HandlerIRQ0x08();
        static void HandlerIRQ0x09();
        static void HandlerIRQ0x0A();
        static void HandlerIRQ0x0B();
        static void HandlerIRQ0x0C();
        static void HandlerIRQ0x0D();
        static void HandlerIRQ0x0E();
        static void HandlerIRQ0x0F();

        static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);




    
    public:

        interruptsHandler();
        ~interruptsHandler();
        void LoadIDT(struct interruptDesciptorTablePointer);


};


#endif // INTERRUPTS_H