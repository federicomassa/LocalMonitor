#ifndef DYNAMIC_MODELS_H
#define DYNAMIC_MODELS_H

#include <string>
#include <cstdlib>
#include <iostream>
#include "Basic/Agent.h"
#include "Automation/Control.h"
#include "Automation/DynamicModel.h"

DynamicsFcn GetDynamicsFunction(const std::string& fcnName);

#endif
