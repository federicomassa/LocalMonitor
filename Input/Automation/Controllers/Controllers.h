#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <string>
#include <iostream>

#include "Automation/Controller.h"

Controller* InstantiateController(const std::string& controllerClassName);

#endif
