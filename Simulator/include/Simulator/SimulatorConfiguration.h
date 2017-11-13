#ifndef SIMULATORCONFIGURATION_H
#define SIMULATORCONFIGURATION_H

#include <fstream>
#include <string>
#include <set>

#include "SimulAgent.h"
#include "SimulationParameters.h"
#include "json.hpp"


class SimulatorViewer;

/**
 * @brief Class that contains the info parsed from Simulator Config file (Input/Simulator.json)
 *
 */
class SimulatorConfiguration
{
	enum EntryType {MANDATORY, OPTIONAL};
	
	class Entry
	{
		friend class SimulatorConfiguration;
		const EntryType type;
		const std::string name;
	public:
		Entry(const std::string& n, const EntryType& t = OPTIONAL) : type(t), name(n)
		{}
		bool operator<(const Entry& e) const
		{
			return (name < e.name);
		}
		bool operator==(const Entry& e) const
		{
			return (name == e.name);
		}
	};
		
	nlohmann::json j;
	std::set<Entry> entries;
    SimulAgentVector agents;
    double simulTimeSpan;
    double simulTimeStep;
    double simulRealTimeSpan;
	SimulatorViewer* simulatorViewer;
    int simulSteps;
	bool useSimulatorViewer;
	SimulationParameters parameters;
	std::map<std::string, AgentParameters> agentsParameters;

	/**
	 * @brief Register a primitive-type entry
	 * 
	 * @param entryName p_entryName: Name of the entry in json file
	 * @param type p_type: if it is mandatory or not;
	 */
	void RegisterEntry(const std::string& entryName, const EntryType& type = OPTIONAL);
	
	
	/**
	 * @brief Returns a json containing the specific entry
	 * 
	 * @param entryName p_entryName:entry name
	 * @return nlohmann::json
	 */
	nlohmann::json GetEntry(const std::string& entryName) const;
	
    SimulAgent ReadAgent ( const nlohmann::json & );
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
	const AgentParameters& GetAgentParameters(const std::string& ID) const;
	const bool& UseSimulatorViewer() const;

};

#endif
