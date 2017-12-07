#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <string>
#include <iostream>

class Controller;

Controller* InstantiateController(const std::string& controllerClassName);

#endif
