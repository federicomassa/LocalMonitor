#ifndef POWNET_CONTROLLER_H
#define POWNET_CONTROLLER_H

#include "Automation/Controller.h"

class PowNetController : public Controller 
{
public:
	PowNetController(const std::string& className);
	void ComputeControl(Control&, const Maneuver&) const override;
};


#endif
