#ifndef POWNET_DISTR_AUTOMATON_H
#define POWNET_DISTR_AUTOMATON_H

#include "Automation/Automaton.h"
#include <string>

class PowNetDistrAutomaton : public Automaton
{
public:
	PowNetDistrAutomaton(const std::string& className);
	void DefineRules() override;
};

#endif
