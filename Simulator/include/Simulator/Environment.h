#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "SimulatorConfiguration.h"
#include "SimulAgent.h"

#include "Utility/SystemTypes.h"
#include "Basic/Agent.h"
#include "Basic/StateRegion.h"

class Environment
{
    SimulAgentVector agents;
    StateRegion ( *invisibleRegionFcn ) ( Agent, AgentVector );
public:
    Environment ();

    const SimulAgentVector &GetAgents() const;
    /**
     * @brief Invoked at each simulation step, evolves the system
     *
     */
    void Run();

    void Configure ( const SimulatorConfiguration & );
    /**
     * @brief Calculate invisible region of state space
     */
    StateRegion InvisibleRegion ( Agent, AgentVector );
};

#endif
