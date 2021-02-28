/*
    pci.cpp - Peripheral Component Interconnect driver implementation file
*/

#include "drivers/pci.h"

// TODO: remove these and namespace variables
using namespace morphios::common;
using namespace morphios::drivers;
using namespace morphios::kernel;

// Constructor
PCIController::PCIController() : addressPort(0xCF8), dataPort(0xCFC) {}

// Destructor
PCIController::~PCIController(){}

// Uses a brute force approach to find PCI devices
void PCIController::findDevices(DriverManager *drv){
    for (uint8_t _bus = 0; _bus < 1; _bus++) {
        for (uint8_t _slot = 0; _slot < 4; _slot++) {
            for (uint8_t _func = 0; _func < 8; _func++) {
                PCIDevice dev = getPCIDeviceInfo(_bus, _slot, _func);

                // Function does not exist, continue to check non-contiguous functions
                if (dev.vendorID == 0x0000 || dev.vendorID == 0xFFFF)
                    continue;
                
                // Enumerate base address registers
                for (uint32_t _bar = 0; _bar < 6; _bar++) {
                    BaseAddressRegister bar = getBaseAddressRegister(_bus, _slot, _func, _bar);
                    if (bar.address && (bar.type == IO_MAPPED))
                        dev.portBase = (uint32_t)bar.address;

                    Driver *driver = getDriver(dev);

                    if (driver != 0)
                        drv->addDriver(driver);
                }

                // Debug prints
                // kprintf("PCI BUS ");
                // kprintHex(_bus & 0xFF);
                // kprintf(", DEVICE ");
                // kprintHex(_slot & 0xFF);
                // kprintf(", FUNCTION ");
                // kprintHex(_func & 0xFF);                
                // kprintf(" = VENDOR ");
                // kprintHex((dev.vendorID & 0xFF00) >> 8);
                // kprintHex(dev.vendorID & 0xFF);
                // kprintf(", DEVICE ");
                // kprintHex((dev.deviceID & 0xFF00) >> 8);
                // kprintHex(dev.deviceID & 0xFF);
                // kprintf("\n");
            }
        }
    }
}

// Check if the device has more than 1 function
// bool PCIController::hasMultipleFunctions( uint16_t bus, uint16_t slot) {
//     return read(bus, slot,0, 0xE) & (1 << 7); // Return bit 7 which says if multiple functions are supported
// }

// Creates a PCIDevice object and retrieves the config space info
PCIDevice PCIController::getPCIDeviceInfo(uint16_t bus, uint16_t slot, uint16_t function) {
    PCIDevice dev;
    dev.bus = bus;
    dev.slot = slot;
    dev.function = function;

    dev.vendorID = read(bus, slot, function, 0x00);
    dev.deviceID = read(bus, slot, function, 0x02);
    dev.classCode = read(bus, slot, function, 0x0B);
    dev.subClass = read(bus, slot, function, 0x0A);
    dev.progIF = read(bus, slot, function, 0x09);
    dev.revisionID = read(bus, slot, function, 0x08);
    dev.interrupt = read(bus, slot, function, 0x3C);

    return dev;
}

// Read data from a PCI register
uint32_t PCIController::read(
    uint16_t bus,
    uint16_t slot, 
    uint16_t function,
    uint32_t registerOffset){
    
    // Config address
    uint32_t addr = (uint32_t)(1 << 31 
                    | ((bus & 0xFF) << 16) 
                    | ((slot & 0x1F) << 11)
                    | ((function & 0x07) << 8)
                    | (registerOffset & 0xFC));
    addressPort.write(addr);

    // Read result
    uint32_t result = dataPort.read(); // result returns the full 4-byte aligned word.
    return result >> (8 * (registerOffset % 4));
}

// Link Driver to PCI device
Driver *PCIController::getDriver(PCIDevice dev){
    switch(dev.vendorID) {
        case 8086: // Intel
        break;

        default:
        break;
    }

    switch(dev.classCode) {
        
        case 0x03: // Display controller
        switch (dev.subClass) {
        
            case 0x00: // VGA
            // kprintf("VGA");
            break;
            
            default:
                break;
        }
    }

    return 0;
}

// Get address space of PCI device
BaseAddressRegister PCIController::getBaseAddressRegister(
    common::uint16_t bus,
    common::uint16_t slot, 
    common::uint16_t function,
    common::uint16_t bar) {
    
    BaseAddressRegister reg;
    uint32_t headerType = read(bus, slot, function, 0xE) & 0x7F;
    int maxBARs = 6 - (4 * headerType);
    if (bar >= maxBARs)
        return reg;
    
    uint32_t barValue = read(bus, slot, function, 0x10 + 4*bar);
    reg.type = (barValue & 0x1) ? IO_MAPPED : MEMORY_MAPPED;

    if (reg.type == MEMORY_MAPPED) {
        switch ((barValue >> 1) & 0x3)
        {
        case 0: // 32-bit mode
        case 1: // 20-bit mode
        case 2: // 64-bit mode
        
        default:
            break;
        }
    }
    else {
        reg.address = (uint8_t *)(barValue & ~0x3);
        reg.prefetchable = false;
    }

    return reg;
}


// Write data to PCI register
void PCIController::write(
    uint16_t bus,
    uint16_t slot, 
    uint16_t function,
    uint32_t registerOffset,
    uint32_t value){

    // Config address
    uint32_t addr = (uint32_t)(1 << 31 
                    | ((bus & 0xFF) << 16) 
                    | ((slot & 0x1F) << 11)
                    | ((function & 0x07) << 8)
                    | (registerOffset & 0xFC));
    addressPort.write(addr);
    dataPort.write(value);
}

PCIDevice::PCIDevice(){}

PCIDevice::~PCIDevice(){}

