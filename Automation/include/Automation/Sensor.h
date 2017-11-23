#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <vector>
#include "Basic/StateRegion.h"

class Sensor
{
	typedef std::vector<std::string> MeasuredVars;
	MeasuredVars measuredVars;
	
	
protected:
	virtual StateRegion ComputeVisibility();
	
};

#endif
