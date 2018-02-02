#ifndef CONTROLLER_FAILURE_H
#define CONTROLLER_FAILURE_H

// State --> [Controller] --> Control --> [Failure] --> Control
// ControlFailure acts as a filter that takes the control as input and does something to it before it actuates

#include "Control.h"
#include "Basic/Maneuver.h"
#include <string>

typedef void (*ControllerFailureFcn)(Control&, const Maneuver&, const double&);

class ControllerFailure
{
public:
private:
	std::string name;
	ControllerFailureFcn fcn;
	
public:
	ControllerFailure();
	ControllerFailure(const ControllerFailure&);
	const ControllerFailure& operator=(const ControllerFailure&);
	void SetFailureFunction(ControllerFailureFcn);
	void SetName(const std::string&);
	const std::string& GetName() const;
	void Run(Control&, const Maneuver&, const double& time) const;
};

#endif
