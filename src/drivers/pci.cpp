/*
    pci.cpp - Peripheral Component Interconnect driver implementation file
*/

#include "drivers/pci.h"

using namespace morphios::common;
using namespace morphios::drivers;
using namespace morphios::kernel;

// Constructor
PCIController::PCIController() : addressPort(0xCF8), dataPort(0xCFC) {}

// Destructor
PCIController::~PCIController(){}

// Uses a brute force approach to find PCI devices
void PCIController::findDevices(){
    for (uint8_t _bus = 0; _bus < 8; _bus++) {
        for (uint8_t _slot = 0; _slot < 32; _slot++) {

            // Check if device exists
            PCIDevice dev = getPCIDeviceInfo(_bus, _slot, 0);
            
            // Find the number of functions
            uint8_t numFunctions = findDeviceFunctions(_bus, _slot);

            for (uint8_t _func = 0; _func < numFunctions; _func++) {
                // Create a PCIDevice object
                dev.bus = _bus;
                dev.slot = _slot;
                dev.function = _func;
            }





        }
    }
}

// Read data from a PCI register
uint32_t PCIController::read(
    uint16_t bus,
    uint16_t slot, 
    uint16_t function,
    uint32_t registerOffset){
    
    // Config address to read
    uint32_t addr = (uint32_t) 0x1 << 31 
                    | ((bus & 0xFF) << 16) 
                    | ((slot & 0x1F) << 11)
                    | ((function & 0x07) << 8)
                    | (registerOffset << 0xFC);
    addressPort.write(addr);

    // Read result
    uint32_t result = dataPort.read();
    return (result >> (8 * (registerOffset % 4)));
}

// Write data to PCI register
void PCIController::write(
    uint16_t bus,
    uint16_t slot, 
    uint16_t function,
    uint32_t registerOffset,
    uint32_t value){

}

// Creates a PCIDevice object and retrieves the config space info
PCIDevice PCIController::getPCIDeviceInfo(
    uint16_t bus,
    uint16_t slot,
    uint16_t function) {
        
}

// Returns true if the device has 1 or more functions
uint8_t PCIController::findDeviceFunctions(uint16_t bus, uint16_t slot) {

}


PCIDevice::PCIDevice(){}

PCIDevice::~PCIDevice(){}

