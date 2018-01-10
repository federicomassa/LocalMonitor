#ifndef STATE_CONVERSIONS_H
#define STATE_CONVERSIONS_H

#include <string>
#include <iostream>
#include "Basic/Agent.h"
#include "Automation/DynamicModel.h"

StateConversionFcn GetStateConversionFunction(const std::string& fcnName);

#endif
