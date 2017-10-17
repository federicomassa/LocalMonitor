#ifndef SIMULATORCONFIGURATION_H
#define SIMULATORCONFIGURATION_H

#include <fstream>

#include "SimulAgent.h"
#include "json.hpp"


/**
 * @brief Class that contains the info parsed from Simulator Config file (Input/Simulator.json)
 *
 */
class SimulatorConfiguration
{
    std::ifstream file;
    SimulAgentVector agents;
    double simulTimeSpan;
    double simulTimeStep;
    double simulRealTimeSpan;
    int simulSteps;

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


};

#endif
