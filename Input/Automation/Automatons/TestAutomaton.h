#ifndef TEST_AUTOMATON_H
#define TEST_AUTOMATON_H

#include "Automation/Automaton.h"
#include <string>

class TestAutomaton : public Automaton
{
	Maneuver oldManeuver;
public:
	TestAutomaton(const std::string& className);
	void DefineRules() override;

	void PreEvolve() override;
	void PostEvolve() override;
};

#endif
