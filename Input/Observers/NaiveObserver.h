#ifndef NAIVE_OBSERVER_H
#define NAIVE_OBSERVER_H

#include "Observer/Observer.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Controller.h"
#include "Automation/ExternalSensor.h"
#include "Automation/InternalSensor.h"
#include "Automation/Automaton.h"
#include "json.hpp"
#include <string>
#include "Utility/IMap.h"
#include <utility>
#include <memory>

class NaiveObserver : public Observer
{
	// Each agent in the environment, including the hidden one, has its own dynamics and controller (usually only the observed one is complicated, and the others are simplified
	IMap<PhysicalLayer> pLayer;
	IMap<Automaton*> automaton;
	IMap<Controller*> controller;
	
	// ID of observed agent
	std::string observedID;
	
	// This is the discretization step. Prediction is achieved by integrating q_dot in this time
	int simulTimeStep;
	
	// This is the total prediction time span, that is, the time interval from prediction to update 
	int predictionTimeSpan;
	
	// This is the mapping range FIXME right now the mapping
	// is done in this range, without considering visibility
	IMap<std::pair<double, double> > varRange;
	
	// This is the mapping resolution
	IMap<double> varResolution;
	
	// Number of predictions per variable. Obtained from varRange and varResolution
	IMap<int> nPredictions;
	
	// These are the initial values of the hidden vehicle (their number depend on var range and resolution. They change at each new prediction
	std::vector<State> hiddenInitState;
	
	// Assumed sensors of observed agent, like in SimulAgent
	std::vector<std::shared_ptr<ExternalSensor> > extSensors;
	std::vector<std::shared_ptr<InternalSensor> > intSensors;

	ExternalSensorOutput RetrieveExternalSensorOutput(const std::string& sensorName,
		const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, 
		const EnvironmentParameters& trueEnvParams);
	
	InternalSensorOutput RetrieveInternalSensorOutput(const std::string& sensorName,
		const Agent& trueSelfInWorld);
	
	
	void ReadDynamicModel(const nlohmann::json&);
	void ReadControlModel(const nlohmann::json&);
	void ReadRange(const nlohmann::json&);
	void ReadResolution(const nlohmann::json&);
	SensorOutput SimulateSensors(const Agent&, const AgentVector&, const EnvironmentParameters&);

	void InitializeHiddenState();

public:
	NaiveObserver(const std::string& name);
	~NaiveObserver();
	void Run() override;
	void Configure(const nlohmann::json&) override;
	void ReceiveSensorOutput(const SensorOutput&, const double& currentTime) override;

	
};

#endif
