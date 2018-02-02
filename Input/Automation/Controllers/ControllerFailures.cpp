#include "ControllerFailures.h"
#include "ZeroControlFailure.h"
#include <iostream>

ControllerFailureFcn InstantiateControllerFailure(const std::string& fcnName)
{
	if (fcnName == "ZeroControlFailure")
		return &ZeroControlFailure;
	else
	{
		std::cerr << "Error in InstantiateControllerFailure: " << fcnName << ".h not found";
		exit(1);
	}
}

