#include "Controllers.h"

#include "Input/Automation/Controllers/AccOmegaControl.h"

Controller* InstantiateController(const std::string& controllerClassName) 
{
	if (controllerClassName == "AccOmegaControl")
		return new AccOmegaControl;
	else 
		{
			std::cerr << "Error in configure: " << controllerClassName << ".h not found";
			exit(1);
		} 
}
