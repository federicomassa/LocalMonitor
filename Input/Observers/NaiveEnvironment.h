#ifndef NAIVE_ENVIRONMENT_H
#define NAIVE_ENVIRONMENT_H

// This object contains a complete prediction, 

#include "Basic/Agent.h"
#include "Utility/EnvironmentParameters.h"
#include "Automation/SensorOutput.h"
#include "Basic/Maneuver.h"
#include "Utility/IMap.h"
#include "Automation/PhysicalLayer.h"
#include <memory>

class ExternalSensor;
class InternalSensor;
class NaiveObserver;
class Controller;
class Automaton;

class NaiveEnvironment
{
	friend class NaiveObserver;
	
	NaiveObserver* observer;
	
	const std::vector<std::shared_ptr<ExternalSensor> >& extSensors;
	const std::vector<std::shared_ptr<InternalSensor> >& intSensors;
	
	Agent self;
	Maneuver selfManeuver;
	AgentVector others;
	EnvironmentParameters env;
	
	// In local coordinates (state variables)
	Agent localSelf;
	AgentVector localOthers;
	
	bool hasHiddenAgent;
		
	// a pLayer for each agent, an automaton just for the observed (the others have a simple model), a controller for each one (but simple for agents != observedID)
	IMap<PhysicalLayer> pLayer;
	std::shared_ptr<Automaton> automaton;
	IMap<std::shared_ptr<Controller> > controller;
	
	// This methods are used to simulate sensors of observed agent
	SensorOutput SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams);
	
	ExternalSensorOutput RetrieveExternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams);
	
	InternalSensorOutput RetrieveInternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld);
	
public:
	NaiveEnvironment(NaiveObserver* parent, const Agent&, const Maneuver&, const AgentVector&, const EnvironmentParameters&,
		const std::vector<std::shared_ptr<ExternalSensor> >&, const std::vector<std::shared_ptr<InternalSensor> >&, const bool& hidden = true
	);
	
	~NaiveEnvironment();
	NaiveEnvironment(const NaiveEnvironment&);
	const NaiveEnvironment& operator=(const NaiveEnvironment&);
	
	
	void Predict(const double& predictionSpan);
	
	
};

#endif
