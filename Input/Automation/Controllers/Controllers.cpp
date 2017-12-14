#include "Controllers.h"
#include "AccOmegaControl.h"
#include <iostream>

Controller* InstantiateController(const std::string& controllerClassName) 
{
	if (controllerClassName == "AccOmegaControl")
	{
		return new AccOmegaControl(controllerClassName);
	}
	else 
		{
			std::cerr << "Error in configure: " << controllerClassName << ".h not found";
			exit(1);
		} 
}
