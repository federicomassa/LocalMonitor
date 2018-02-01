#ifndef SIMULATORCONFIGURATION_H
#define SIMULATORCONFIGURATION_H

#include <fstream>
#include <string>
#include <set>

#include "SimulAgent.h"
#include "Utility/WorldFeatures.h"
#include "Utility/SimulationParameters.h"
#include "Utility/EnvironmentParameters.h"
#include "Automation/DynamicModel.h"
#include "Automation/ControlModel.h"

#include "Automation/InternalSensor.h"
#include "Automation/ExternalSensor.h"
#include "json.hpp"

class ControlModel;
class SimulatorViewer;

/**
 * @brief Class that contains the info parsed from Simulator Config file (Input/Simulator.json)
 *
 */
class SimulatorConfiguration
{
	MyLogger* logger;
	
	nlohmann::json j;
	std::set<std::string> mandatoryEntries;
	
	// This includes mandatory entries. Everything not standard is custom and goes to parameters. With 'standard' we only mean that it has a special meaning for the software
	std::set<std::string> standardEntries;
	
    SimulAgentVector agents;
    double simulTimeSpan;
    double simulTimeStep;
    double simulRealTimeSpan;
	SimulatorViewer* simulatorViewer;
    int simulSteps;
	bool useSimulatorViewer;
	
	// Non numerical custom parameters
	SimulationParameters simCustomEntries;
	// Numerical, potentially measurable, environment parameters
	EnvironmentParameters envParameters;
	
	std::map<std::string, AgentCustomEntries> agentsCustomEntries;
	
	std::set<DynamicModel> dynamicModels;
	std::set<ControlModel> controlModels;
	std::set<std::string> extSensors;
	std::set<std::string> intSensors;
	
	WorldEnvironmentFeatures envFeatures;
	WorldAgentFeatures agentFeatures;

	/**
	 * @brief Register a primitive-type entry
	 * 
	 * @param entryName p_entryName: Name of the entry in json file
	 * @param type p_type: if it is mandatory or not;
	 */
	void RegisterMandatoryEntry(const std::string& entryName);
	void RegisterStandardEntry(const std::string& entryName);
	
    SimulAgent ReadAgent ( const nlohmann::json & );
	void ReadSensor(const nlohmann::json&);
	void AddDynamicModel ( const nlohmann::json & );
	void AddControlModel ( const nlohmann::json & );

	//Sensing ReadSensing(const nlohmann::json& agent);
public:
    /**
     * @brief Constructor with configuration file path
     *
     */
    SimulatorConfiguration ( const std::string & , MyLogger* log = nullptr);

    /**
     * @brief Extract information from configuration file
     *
     */
    void Parse();

    const SimulAgentVector &GetAgents() const;

    const int &GetSimulationSteps() const;
	const double &GetSimulationTimeStep() const;
	const SimulationParameters& GetCustomEntries() const;
	const double& GetEnvironmentParameter(const std::string& parName) const;		const EnvironmentParameters& GetEnvironmentParameters() const;

	const AgentCustomEntries& GetAgentCustomEntries(const std::string& ID) const;
	const WorldAgentFeatures& GetWorldAgentFeatures() const;
	const WorldEnvironmentFeatures& GetWorldEnvironmentFeatures() const;
	const bool& UseSimulatorViewer() const;

	static nlohmann::json GetEntry(const std::string& entryName, const nlohmann::json&);
	
	/**
	 * @brief Returns a json containing the specific entry
	 * 
	 * @param entryName p_entryName:entry name
	 * @return nlohmann::json
	 */
	 nlohmann::json GetEntry(const std::string& entryName) const;
	
};

#endif
