#ifndef STATE_CONVERSIONS_H
#define STATE_CONVERSIONS_H

#include <string>
#include <iostream>
#include "Basic/Agent.h"
#include "TestKinematicsConversion.h"

typedef void (*StateConversionFcn) (State&, const Agent&);

StateConversionFcn GetStateConversionFunction(const std::string& fcnName) 
{
	if (fcnName == "TestKinematicsConversion")
		return &TestKinematicsConversion;
	else 
		{
			std::cerr << "Error in configure: " << fcnName << ".h not found";
			exit(1);
		} 
}

#endif