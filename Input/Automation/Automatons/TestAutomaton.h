#ifndef TEST_AUTOMATON_H
#define TEST_AUTOMATON_H

#include "Automation/Automaton.h"
#include <string>

class TestAutomaton : public Automaton
{
public:
	TestAutomaton(const std::string& className);
	void DefineRules() override;
};

#endif
