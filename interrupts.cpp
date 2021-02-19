/*
    interrupts.cpp - x86 interrupt handler in C++
*/

#include "interrupts.h"

// Constructor
interruptHandle::interruptHandle(interruptsHandler *InterruptsHandler, uint8_t IRQ) {
    this->IRQ = IRQ;
    this->InterruptsHandler = InterruptsHandler;
    InterruptsHandler->interruptHandles[IRQ] = this;
}

// Destructor
interruptHandle::~interruptHandle() {
    if(InterruptsHandler->interruptHandles[IRQ] == this)
        InterruptsHandler->interruptHandles[IRQ] = 0;
} 

// Default ISR
uint32_t interruptHandle::ISR(uint32_t esp) {
    return esp;
}

// IDT
interruptsHandler::Gate interruptsHandler::interruptDescriptorTable[256];

// Active IRQ handler
interruptsHandler* interruptsHandler::ActiveInterruptsHandler = 0;

// Create entry in IDT
void interruptsHandler::SetInterruptDescriptorTableEntry(uint8_t IRQ,
    uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType) {
    // address of pointer to code segment (relative to global descriptor table)
    // and address of the handler (relative to segment)
    interruptDescriptorTable[IRQ].offsetLo = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[IRQ].offsetHi = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[IRQ].selector = CodeSegment;
    interruptDescriptorTable[IRQ].typeAttr = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 0x3) << 5) | DescriptorType;
    interruptDescriptorTable[IRQ].zero = 0;
}

// IDT constructor
interruptsHandler::interruptsHandler(GlobalDescriptorTable* globalDescriptorTable) 
    : PICMasterCommandPort(PIC_MASTER_COMMAND_PORT),
    PICSlaveCommandPort(PIC_SLAVE_COMMAND_PORT),
    PICMasterDataPort(PIC_MASTER_DATA_PORT),
    PICSlaveDataPort(PIC_SLAVE_DATA_PORT) {
    
    uint32_t CodeSegment = globalDescriptorTable->CodeSegmentSelector();

    // Initialise all interrupts to default (do nothing)
    for (uint8_t i = 255; i > 0; i--) {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
        interruptHandles[i] = 0;
    }
    interruptHandles[0] = 0;
    // Set first 32 IRQs to system exceptions
    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandlerException0x00, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandlerException0x01, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandlerException0x02, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandlerException0x03, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandlerException0x04, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandlerException0x05, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandlerException0x06, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandlerException0x07, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandlerException0x08, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandlerException0x09, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandlerException0x0A, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandlerException0x0B, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandlerException0x0C, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandlerException0x0D, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandlerException0x0E, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandlerException0x0F, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandlerException0x10, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandlerException0x11, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandlerException0x12, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x14, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x15, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x16, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x17, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x18, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x19, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1A, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1B, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1C, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1D, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1E, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1F, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);

    // User defined PIC interrupts
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x00, CodeSegment, &HandlerIRQ0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x01, CodeSegment, &HandlerIRQ0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x02, CodeSegment, &HandlerIRQ0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x03, CodeSegment, &HandlerIRQ0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x04, CodeSegment, &HandlerIRQ0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x05, CodeSegment, &HandlerIRQ0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x06, CodeSegment, &HandlerIRQ0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x07, CodeSegment, &HandlerIRQ0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x08, CodeSegment, &HandlerIRQ0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x09, CodeSegment, &HandlerIRQ0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0A, CodeSegment, &HandlerIRQ0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0B, CodeSegment, &HandlerIRQ0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0C, CodeSegment, &HandlerIRQ0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0D, CodeSegment, &HandlerIRQ0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0E, CodeSegment, &HandlerIRQ0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0F, CodeSegment, &HandlerIRQ0x0F, 0, IDT_INTERRUPT_GATE);

    // Remap the PIC
    PICMasterCommandPort.write(0x11);
    PICSlaveCommandPort.write(0x11);

    PICMasterDataPort.write(HW_INTERRUPT_OFFSET);
    PICSlaveDataPort.write(HW_INTERRUPT_OFFSET+8);

    PICMasterDataPort.write(0x04);
    PICSlaveDataPort.write(0x02);

    PICMasterDataPort.write(0x01);
    PICSlaveDataPort.write(0x01);

    PICMasterDataPort.write(0x00);
    PICSlaveDataPort.write(0x00);

    // Load IDT register
    interruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(Gate) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
}

// Destructor
interruptsHandler::~interruptsHandler(){
    Deactivate();
}

// Activate IRQs
void interruptsHandler::Activate() {
    if (ActiveInterruptsHandler != 0)
        ActiveInterruptsHandler->Deactivate();

    ActiveInterruptsHandler = this;
    asm("sti");
}

// Disable IRQs
void interruptsHandler::Deactivate() {
    if (ActiveInterruptsHandler == this) {
        ActiveInterruptsHandler = 0;
        asm("cli");
    }
}

// IRQ handler
uint32_t interruptsHandler::HandleInterrupt(uint8_t IRQ, uint32_t esp) {
    if(ActiveInterruptsHandler != 0)
        return ActiveInterruptsHandler->DoHandleInterrupt(IRQ, esp);
    return esp;
}

uint32_t interruptsHandler::DoHandleInterrupt(uint8_t IRQ, uint32_t esp) {    
    if(interruptHandles[IRQ] != 0) // Handled IRQ
    {
        esp = interruptHandles[IRQ]->ISR(esp);
    }
    else if(IRQ != HW_INTERRUPT_OFFSET) // Unhandled IRQ
    {
        char* message = "UNHANDLED INTERRUPT 0x00";
        char* hex = "0123456789ABCDEF";
        message[22] = hex[(IRQ >> 4) & 0xF];
        message[23] = hex[IRQ & 0xF];
        kprintf(message);
    } 
    else {} // PIC timer IRQ

    // EOI (End of Interrupt) command to PICs
    if(IRQ >= HW_INTERRUPT_OFFSET && IRQ < HW_INTERRUPT_OFFSET + 16) {
        PICMasterCommandPort.write(0x20);
        if(HW_INTERRUPT_OFFSET + 8 <= IRQ)
            PICSlaveCommandPort.write(0x20);
    }

    return esp;
}

// Handler for unhandled IRQs
void interruptsHandler::InterruptIgnore(){
    asm volatile("iret");
}

