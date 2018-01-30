#ifndef STRAIGHT_LINE_CONTROL_H
#define STRAIGHT_LINE_CONTROL_H

#include "Automation/Controller.h"

class StraightLineControl : public Controller 
{
public:
	StraightLineControl(const std::string& className);
	void ComputeControl(Control&, const Maneuver&) const override;
};


#endif
