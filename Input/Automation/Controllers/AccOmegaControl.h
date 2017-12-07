#ifndef ACC_OMEGA_CONTROL_H
#define ACC_OMEGA_CONTROL_H

#include "Automation/Controller.h"

class AccOmegaControl : public Controller 
{
public:
	AccOmegaControl(const std::string& className);
	void ComputeControl(Control&, const Maneuver&) const override;
};


#endif
