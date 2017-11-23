#include "AccOmegaControl.h"

Control AccOmegaControl(const State& q0 ,const Maneuver& maneuver)
{
	Control u;
	
	const double K = 10;
	
	if (maneuver == "FAST")
	{
		u["a"] = 1;
		u["omega"] = -(q0.at("y") - 
	}
	
	return u;
}
