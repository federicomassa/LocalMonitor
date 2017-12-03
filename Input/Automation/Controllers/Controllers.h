#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <string>
#include <iostream>
#include "AccOmegaControl.h"

Controller* InstantiateController(const std::string& controllerClassName);

#endif
