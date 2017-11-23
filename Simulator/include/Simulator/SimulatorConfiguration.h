#ifndef SIMULATORCONFIGURATION_H
#define SIMULATORCONFIGURATION_H

#include <fstream>
#include <string>
#include <set>

#include "SimulAgent.h"
#include "WorldFeatures.h"
#include "SimulationParameters.h"
#include "Automation/DynamicModel.h"
#include "json.hpp"


class SimulatorViewer;

/**
 * @brief Class that contains the info parsed from Simulator Config file (Input/Simulator.json)
 *
 */
class SimulatorConfiguration
{

	nlohmann::json j;
	std::set<std::string> mandatoryEntries;
	
	// This includes mandatory entries. Everything not standard is custom and goes to parameters
	std::set<std::string> standardEntries;
	
    SimulAgentVector agents;
    double simulTimeSpan;
    double simulTimeStep;
    double simulRealTimeSpan;
	SimulatorViewer* simulatorViewer;
    int simulSteps;
	bool useSimulatorViewer;
	SimulationParameters parameters;
	
	std::map<std::string, AgentParameters> agentsCustomEntries;
	
	std::set<DynamicModel> dynamicModels;
	std::set<ControlModel> controlModels;

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

	
	/**
	 * @brief Returns a json containing the specific entry
	 * 
	 * @param entryName p_entryName:entry name
	 * @return nlohmann::json
	 */
	 nlohmann::json GetEntry(const std::string& entryName) const;
	
    SimulAgent ReadAgent ( const nlohmann::json & );
	void AddDynamicModel ( const nlohmann::json & );

	//Sensing ReadSensing(const nlohmann::json& agent);
public:
    /**
     * @brief Constructor with configuration file path
     *
     */
    SimulatorConfiguration ( const std::string & );

    /**
     * @brief Extract information from configuration file
     *
     */
    void Parse();

    const SimulAgentVector &GetAgents() const;

    const int &GetSimulationSteps() const;
	const double &GetSimulationTimeStep() const;
	const SimulationParameters& GetParameters() const;
	const AgentParameters& GetAgentCustomEntry(const std::string& ID) const;
	const WorldAgentFeatures& GetWorldAgentFeatures() const;
	const WorldEnvironmentFeatures& GetWorldEnvironmentFeatures() const;
	const bool& UseSimulatorViewer() const;

	static nlohmann::json GetEntry(const std::string& entryName, const nlohmann::json&);

	
};

#endif
