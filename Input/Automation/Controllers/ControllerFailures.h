#ifndef CONTROLLER_FAILURES
#define CONTROLLER_FAILURES

#include "Automation/ControllerFailure.h"
#include <string>

ControllerFailureFcn InstantiateControllerFailure(const std::string&);

#endif
