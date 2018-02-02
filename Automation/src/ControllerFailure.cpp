#include "ControllerFailure.h"
#include "Utility/LogFunctions.h"

ControllerFailure::ControllerFailure()
{
	fcn = nullptr;
}

ControllerFailure::ControllerFailure(const ControllerFailure& f)
{
	*this = f;
}

const ControllerFailure & ControllerFailure::operator=(const ControllerFailure& f)
{
	name = f.name;
	fcn = f.fcn;
	
	return *this;
}


void ControllerFailure::SetFailureFunction(ControllerFailureFcn f)
{
	fcn = f;
}

void ControllerFailure::SetName(const std::string& n)
{
	name = n;
}	

const std::string & ControllerFailure::GetName() const
{
	return name;
}


void ControllerFailure::Run(Control& c, const Maneuver& man, const double& time) const
{
	if (fcn == nullptr)
		LogFunctions::Error("ControllerFailure::Run", "Call SetFailureFunction first");
	
	fcn(c, man, time);
}


