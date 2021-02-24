/*
    pci.h - Peripheral Component Interconnect driver header file 
*/

#ifndef PCI_H
#define PCI_H

#include "common/types.h"
#include "drivers/driver.h"
#include "kernel/port.h"
#include "kernel/interrupts.h"
#include "common/common.h"

namespace morphios {
namespace drivers {

// PCI device class to store device related info
// TODO: currently supports only 0x00 header type (generic device)
// Add support for other header types (0x01, 0x02)
class PCIDevice {
public:
    common::uint16_t bus, slot, function;
    
    common::uint16_t vendorID, deviceID, command, status, revisionID, 
        progIF, subclass, classCode, IRQLine, IRQPin;

    PCIDevice();
    ~PCIDevice();
};

// Read/write to devices on PCI buses
class PCIController {
protected:
    kernel::Port32Bit addressPort;
    kernel::Port32Bit dataPort;

public:
    PCIController();
    ~PCIController();

    // Find active devices
    void findDevices();

    // Finds the number of functions for a PCI device
    uint8_t findDeviceFunctions(
        common::uint16_t bus,
        common::uint16_t slot);

    // Creates a PCIDevice object and retrieves the config space info
    PCIDevice getPCIDeviceInfo(
        common::uint16_t bus,
        common::uint16_t slot,
        common::uint16_t function);
    
    // Read data from a PCI register
    common::uint32_t read(
        common::uint16_t bus,
        common::uint16_t slot, 
        common::uint16_t function,
        common::uint32_t registerOffset);
    
    // Write data to PCI register
    void write(
        common::uint16_t bus,
        common::uint16_t slot, 
        common::uint16_t function,
        common::uint32_t registerOffset,
        common::uint32_t value);
};

} // namespace drivers
} // namespace morphios

#endif // PCI_H
