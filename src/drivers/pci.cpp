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
void PCIController::findDevices(){
    for (uint8_t _bus = 0; _bus < 1; _bus++) {
        for (uint8_t _slot = 0; _slot < 4; _slot++) {
            for (uint8_t _func = 0; _func < 8; _func++) {
                PCIDevice dev = getPCIDeviceInfo(_bus, _slot, _func);

                // Function does not exist, continue to check non-contiguous functions
                if (dev.vendorID == 0x0000 || dev.vendorID == 0xFFFF)
                    continue;

                kprintf("PCI BUS ");
                kprintHex(_bus & 0xFF);

                kprintf(", DEVICE ");
                kprintHex(_slot & 0xFF);

                kprintf(", FUNCTION ");
                kprintHex(_func & 0xFF);
                
                kprintf(" = VENDOR ");
                kprintHex((dev.vendorID & 0xFF00) >> 8);
                kprintHex(dev.vendorID & 0xFF);
                kprintf(", DEVICE ");
                kprintHex((dev.deviceID & 0xFF00) >> 8);
                kprintHex(dev.deviceID & 0xFF);
                kprintf("\n");
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

