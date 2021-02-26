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
class PCIDevice {
public:
    common::uint8_t revisionID, progIF, subClass, classCode;
    common::uint16_t bus, slot, function, vendorID, deviceID;
    common::uint32_t interrupt, portBase;

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

    // Finds the number of functions for the device
    // bool hasMultipleFunctions(
    //     common::uint16_t bus,
    //     common::uint16_t slot
    // );

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
