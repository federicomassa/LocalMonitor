#include "StateConversions.h"
#include <string>
#include <iostream>
#include "TestKinematicsToWorld.h"
#include "TestKinematicsToState.h"

#include "PowNetToWorld.h"
#include "PowNetToState.h"

StateConversionFcn GetStateConversionToWorld(const std::string& fcnName) 
{
	if (fcnName == "TestKinematicsToWorld")
		return &TestKinematicsToWorld;
	else if (fcnName == "PowNetToWorld")
		return &PowNetToWorld;
	else 
		{
			std::cerr << "Error in configure: " << fcnName << ".h not found";
			exit(1);
		} 
}

StateConversionFcn GetStateConversionToState(const std::string& fcnName) 
{
	if (fcnName == "TestKinematicsToState")
		return &TestKinematicsToState;
	else if (fcnName == "PowNetToState")
		return &PowNetToState;
	else 
		{
			std::cerr << "Error in configure: " << fcnName << ".h not found";
			exit(1);
		} 
}



