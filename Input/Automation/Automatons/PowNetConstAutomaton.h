#ifndef POWNET_CONST_AUTOMATON_H
#define POWNET_CONST_AUTOMATON_H

#include "Automation/Automaton.h"
#include <string>

class PowNetConstAutomaton : public Automaton
{
public:
	PowNetConstAutomaton(const std::string& className);
	void DefineRules() override;
};

#endif
