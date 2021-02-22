/*
    driver.cpp - Generic driver class implementation file
*/

#include "drivers/driver.h"

namespace morphios {
namespace drivers {

// These virtual methods will be overloaded by actual drivers
Driver::Driver() {}
Driver::~Driver(){}
void Driver::activate() {}
void Driver::deactivate() {}
void Driver::reset() {}

DriverManager::DriverManager() {
    numDrivers = 0;
}

DriverManager::~DriverManager() {}

void DriverManager::addDriver(Driver *drv) {
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::activateAll() {
    for (morphios::common::uint8_t i = 0; i < numDrivers; i++) {
        drivers[i]->activate();
    } 
}

} // namespace drivers
} // namespace morphios
