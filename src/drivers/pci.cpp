/*
    pci.cpp - Peripheral Component Interconnect driver implementation file
*/

#include "drivers/pci.h"

using namespace morphios::common;
using namespace morphios::drivers;
using namespace morphios::kernel;

// Returns true if the device has 1 or more functions
bool PCIController::findDeviceFunctions(uint16_t bus, uint16_t slot) {}

PCIController::PCIController(){}
PCIController::~PCIController(){}

// Find active devices
void PCIController::findDevices(){}

// Read data from a PCI register
uint32_t PCIController::read(
    uint16_t bus,
    uint16_t slot, 
    uint16_t function,
    uint32_t registerOffset){}

// Write data to PCI register
void PCIController::write(
    uint16_t bus,
    uint16_t slot, 
    uint16_t function,
    uint32_t registerOffset,
    uint32_t value){}

// Creates a PCIDevice object and retrieves the config space info
PCIDevice PCIController::getPCIDeviceInfo(
    uint16_t bus,
    uint16_t slot,
    uint16_t function){}

PCIDevice::PCIDevice(){}

PCIDevice::~PCIDevice(){}

