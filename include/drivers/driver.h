/*
    driver.h - Generic driver class header file
*/

#ifndef DRIVER_H
#define DRIVER_H

#include "common/types.h"

 // Max number of drivers

// Generic driver with virtual methods for overloading by actual driver implementations.
class Driver {
public:
    Driver();
    ~Driver();

    virtual void activate();
    virtual void deactivate();
    virtual void reset();
};

// Driver manager class to make it easy to enable/disable/add drivers.
class DriverManager {
private:
    static const uint8_t MAX_DRIVERS = 255;
    Driver *drivers[MAX_DRIVERS]; // Array of pointers to drivers
    uint8_t numDrivers; // Represents how many drivers are in use

public:
    DriverManager();
    ~DriverManager();
    void addDriver(Driver *drv);
    void activateAll();
};

#endif // DRIVER_H
