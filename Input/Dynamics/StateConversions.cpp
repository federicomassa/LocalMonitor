#include "StateConversions.h"
#include <string>
#include <iostream>
#include "TestKinematicsConversion.h"

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
