#ifndef DYNAMIC_MODELS_H
#define DYNAMIC_MODELS_H

#include "Basic/Agent.h"
#include "Automation/Control.h"

typedef void (*DynamicsFcn) (State&, const Agent&, const Control&);

DynamicsFcn GetDynamicsFunction(const std::string& fcnName);


#endif
