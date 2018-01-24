#include "StateConversions.h"
#include <string>
#include <iostream>
#include "TestKinematicsToWorld.h"
#include "TestKinematicsToState.h"

StateConversionFcn GetStateConversionToWorld(const std::string& fcnName) 
{
	if (fcnName == "TestKinematicsToWorld")
		return &TestKinematicsToWorld;
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
	else 
		{
			std::cerr << "Error in configure: " << fcnName << ".h not found";
			exit(1);
		} 
}



