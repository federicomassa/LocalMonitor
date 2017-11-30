#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "SimulatorConfiguration.h"
#include "SimulAgent.h"

#include "WorldFeatures.h"

#include "Utility/SystemTypes.h"
#include "Basic/Agent.h"
#include "Basic/StateRegion.h"

class Environment
{
    SimulAgentVector agents;
	WorldAgentFeatures agentFeatures;
	WorldEnvironmentFeatures envFeatures;
	EnvironmentParameters envParameters;
	
	State GenerateWorldAgentState() const;
	State GenerateWorldEnvironmentState() const;
	
public:
    Environment ();

    const SimulAgentVector &GetAgents() const;
    /**
     * @brief Invoked at each simulation step, evolves the system
     *
     */
    void Run(const double& currentTime);
	void Configure ( const SimulatorConfiguration & );
	
	void ConvertAgentsToWorld();
	
};

#endif
