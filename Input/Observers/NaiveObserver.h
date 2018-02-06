#ifndef NAIVE_OBSERVER_H
#define NAIVE_OBSERVER_H

#include "Observer/Observer.h"
#include "NaiveEnvironment.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Controller.h"
#include "Automation/ExternalSensor.h"
#include "Automation/InternalSensor.h"
#include "Utility/MyLogger.h"
#include "Automation/Automaton.h"
#include "json.hpp"
#include <string>
#include "Utility/IMap.h"
#include <utility>
#include <fstream>
#include <memory>

class NaiveObserver : public Observer
{
	friend class NaiveEnvironment;
	
	// Output file
	std::ofstream output;
	std::string outputDir;
	
	// Current time, updated each run
	double now;
	
	MyLogger logger;
	
	// Each agent in the environment, including the hidden one, has its own dynamics and controller (usually only the observed one is complicated, and the others are simplified
	
	// Physical Layer is the same for each environment, automaton and controller have to be in each environment
	IMap<PhysicalLayer> pLayer;
	
	// Save only models. Actual instantiations are in each NaiveEnvironment
	IMap<ControlModel> controlModels;
	
	// This is the discretization step. Prediction is achieved by integrating q_dot in this time
	int simulTimeSteps;
	
	// This is the total prediction time span, that is, the time interval from prediction to update 
	double predictionTimeSpan;
	
	// Last time you begun a prediction
	double lastPredictionStartTime;
	// When the prediction ends
	double lastPredictedTime;
	
	// Last time you updated the prediction
	double lastUpdateTime;
	
	// Array of world vars
	std::vector<std::string> worldAgentVars;

	
	// This is the mapping range FIXME right now the mapping
	// is done in this range, without considering visibility
	IMap<std::pair<double, double> > varRange;
	
	// This is the mapping resolution
	IMap<double> varResolution;
	
	// This is the list of parameters of hidden agent
	IMap<double> parameters;
	
	// This is the tolerance in local variables to achieve the discrete state observation
	IMap<double> tolerances;

	
	// Number of predictions per variable. Obtained from varRange and varResolution
	IMap<int> nPredictions;
	
	// This contains the simulated environments
	std::vector<NaiveEnvironment> environments;
	
	// These are the initial values of the hidden vehicle (their number depend on var range and resolution. They change at each new prediction
	std::vector<State> hiddenInitState;
	
	// Assumed sensors of observed agent, like in SimulAgent
	std::vector<std::shared_ptr<ExternalSensor> > extSensors;
	std::vector<std::shared_ptr<InternalSensor> > intSensors;
	
	
	void ReadDynamicModel(const nlohmann::json&);
	void ReadControlModel(const nlohmann::json&);
	void ReadRange(const nlohmann::json&);
	void ReadResolution(const nlohmann::json&);
	void ReadParameters(const nlohmann::json&);
	void ReadTolerances(const nlohmann::json&);

	
	// Takes decimal integer and, by converting it to a number in base given by the number of predictions for each variable, produces a different initial state
	State GenerateHiddenState(const int&);
	
	// Generate hidden agent mapping
	void InitializeHiddenState();
	
	// Core of NaiveObserver: Prediction Phase
	void PredictPhase();
	// Core of NaiveObserver: Update phase with interpolated measurement to compare with prediction
	void UpdatePhase(const Agent&, const AgentVector&, const EnvironmentParameters&);

	// Interpolating function to sync with received sensor data
	Agent InterpolateSelf(const TimedContainer<Agent>::const_iterator& p1, const TimedContainer<Agent>::const_iterator& p2);
	AgentVector InterpolateOthers(TimedContainer<AgentVector>::const_iterator p1, TimedContainer<AgentVector>::const_iterator p2);
	EnvironmentParameters InterpolateEnvironment(TimedContainer<EnvironmentParameters>::const_iterator p1, TimedContainer<EnvironmentParameters>::const_iterator p2);
	
	
	
	
public:
	NaiveObserver(const std::string& name);
	~NaiveObserver();
	void Run(const double& currentTime) override;
	void Configure(const nlohmann::json&) override;
	void PreConfigure(const nlohmann::json&) override;

	void ReceiveSensorOutput(const SensorOutput&, const double& currentTime) override;

	
};

#endif
