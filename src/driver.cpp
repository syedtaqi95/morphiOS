/*
    driver.cpp - Generic driver class implementation file
*/

#include "driver.h"

// Constructor
Driver::Driver() {}
Driver::~Driver(){}
void Driver::activate() {}
void Driver::deactivate() {}
void Driver::reset() {}

DriverManager::DriverManager() {

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
