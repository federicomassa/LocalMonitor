#ifndef STATE_CONVERSIONS_H
#define STATE_CONVERSIONS_H

#include <string>
#include <iostream>

#include "Basic/Agent.h"

typedef void (*StateConversionFcn) (State&, const Agent&);

StateConversionFcn GetStateConversionFunction(const std::string& fcnName);

#endif
