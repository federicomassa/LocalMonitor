#ifndef SENSORS_H
#define SENSORS_H

#include <string>
#include <iostream>

#include "Automation/InternalSensor.h"
#include "Automation/ExternalSensor.h"

InternalSensor* InstantiateInternalSensor(const std::string& sensorClassName);
ExternalSensor* InstantiateExternalSensor(const std::string& sensorClassName); 

#endif
