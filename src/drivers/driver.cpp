/*
    driver.cpp - Generic driver class implementation file
*/

#include "drivers/driver.h"

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
    for (uint8_t i = 0; i < numDrivers; i++) {
        drivers[i]->activate();
    } 
}
