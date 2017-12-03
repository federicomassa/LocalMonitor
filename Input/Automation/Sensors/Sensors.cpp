#include "Sensors.h"
#include "VisionSensor.h"
#include "SelfSensor.h"

InternalSensor* InstantiateInternalSensor(const std::string& sensorClassName) 
{
	if (sensorClassName == "SelfSensor")
		return new SelfSensor;
	else
	{
		std::cerr << "Error in configure: " << sensorClassName << ".h not found";
		exit(1);
	}
}

ExternalSensor* InstantiateExternalSensor(const std::string& sensorClassName) 
{
	if (sensorClassName == "VisionSensor")
		return new VisionSensor;
	else 
		{
			std::cerr << "Error in configure: " << sensorClassName << ".h not found";
			exit(1);
		} 
}
