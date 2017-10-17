#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <utility>
#include <string>
#include <fstream>

#include "State.h"

class Logger;


class Agent
{
    std::string ID;
    State state;

public:
    Agent();
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
    void SetState ( const State &q );
    friend Logger &operator<< ( Logger &, const Agent & );
};

typedef std::vector<Agent> AgentVector;

Logger &operator<< ( Logger &, const Agent & );


#endif
