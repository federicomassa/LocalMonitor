#include "DynamicModels.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include "Basic/Agent.h"
#include "Automation/Control.h"
#include "TestKinematics.h"


DynamicsFcn GetDynamicsFunction(const std::string& fcnName) 
{
	if (fcnName == "TestKinematics")
		return &TestKinematics;
	else 
		{
			std::cerr << "Error in configure: " << fcnName << ".h not found";
			exit(1);
		} 
}

