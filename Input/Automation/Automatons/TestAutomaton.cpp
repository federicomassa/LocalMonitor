#include "TestAutomaton.h"
#include "Basic/Agent.h"
#include "Utility/EnvironmentParameters.h"

#include <set>
#include <string>

using namespace std;

bool tooFast(const Agent& self, const EnvironmentParameters& env)
{
	if (self("v") > 25.0)
		return true;
	
	return false;
}

bool tooSlow(const Agent& self, const EnvironmentParameters& env)
{
	if (self("v") < 15.0)
		return true;
	
	return false;
}

TestAutomaton::TestAutomaton(const std::string& className) : Automaton(className)
{
}


void TestAutomaton::DefineRules()
{
	RegisterSubEvent("TooFast", tooFast, SubEvent::SINGLE, "Vehicle is moving too fast");
	RegisterSubEvent("TooSlow", tooSlow, SubEvent::SINGLE, "Vehicle is moving too slowly");
	
	set<string> subs1;
	subs1.insert("TooFast");
	RegisterEvent("TooFast", subs1, "Vehicle is moving too fast");
	
	set<string> subs2;
	subs2.insert("TooSlow");
	
	RegisterEvent("TooSlow", subs2, "Vehicle is moving too slowly");

	set<string> ev1;
	ev1.insert("TooFast");
	AddTransition("FAST", "SLOW", ev1);
	
	set<string> ev2;
	ev2.insert("TooSlow");
	AddTransition("SLOW", "FAST", ev2);
}

