#ifndef POWNET_CONSTANT_CONTROLLER_H
#define POWNET_CONSTANT_CONTROLLER_H

#include "Automation/Controller.h"

class PowNetConstantController : public Controller 
{
public:
	PowNetConstantController(const std::string& className);
	void ComputeControl(Control&, const Maneuver&) const override;
};


#endif
