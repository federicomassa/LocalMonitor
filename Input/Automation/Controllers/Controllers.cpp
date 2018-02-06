#include "Controllers.h"
#include "AccOmegaControl.h"
#include "StraightLineControl.h"
#include "PowNetController.h"
#include "PowNetConstantController.h"
#include <iostream>

Controller* InstantiateController(const std::string& controllerClassName) 
{
	if (controllerClassName == "AccOmegaControl")
	{
		return new AccOmegaControl(controllerClassName);
	}
	else if (controllerClassName == "StraightLineControl")
	{
		return new StraightLineControl(controllerClassName);
	}
	else if (controllerClassName == "PowNetController")
	{
		return new PowNetController(controllerClassName);
	}
	else if (controllerClassName == "PowNetConstantController")
		return new PowNetConstantController(controllerClassName);
	else 
		{
			std::cerr << "Error in configure: " << controllerClassName << ".h not found";
			exit(1);
		} 
}
