#include "ZeroControlFailure.h"

void ZeroControlFailure(Control& control, const Maneuver&, const double& time)
{
	if (time > 0.1 && time < 0.2)
	{
		control("u1") = 1;
		control("u2") = 0.0;
	}
}

