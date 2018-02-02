#include "PowNetDistrAutomaton.h"

#include "Basic/Agent.h"
#include "Utility/EnvironmentParameters.h"
#include "Utility/Math.h"

#include <math.h>
#include <map>
#include <set>
#include <iostream>
#include <string>

using namespace std;

bool sameSign(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	if (self("xc")*(self("V") - 1.4941) > 0)
		return true;
	
	return false;
}

bool differentSign(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	if (self("xc")*(self("V") - 1.4941) < 0)
		return true;
	
	return false;
}

bool tooMuchEnergy(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	if (pow(self("xc"), 2) > pow(self("V") - 1.4941, 2))
		return true;
	
	return false;
}

bool smallStabilizer(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	if (fabs(self("xc")) < 1E-4)
		return true;
	
	return false;
}

PowNetDistrAutomaton::PowNetDistrAutomaton(const std::string& className) : Automaton(className)
{
}


void PowNetDistrAutomaton::DefineRules()
{
	// ============= REGISTER SUB-EVENTS ===================
	
	RegisterSubEvent("SameSign", sameSign, SubEvent::SINGLE, "Stabilizer and distance from desired tension have same sign");
	RegisterSubEvent("NotSameSign", differentSign, SubEvent::SINGLE, "Stabilizer and distance from desired tension have different sign");
	RegisterSubEvent("TooMuchEnergy", tooMuchEnergy, SubEvent::SINGLE, "Stabilizer is pouring too much energy");
	RegisterSubEvent("SmallStabilizer", smallStabilizer, SubEvent::SINGLE, "Stabilizer is approx at zero value");
	
	// ============== REGISTER EVENTS ======================
	
	set<string> sameSignEvnt;
	sameSignEvnt.insert("SameSign");
	RegisterEvent("SameSign", sameSignEvnt, "Stabilizer and distance from desired tension have same sign");
	
	
	set<string> nSameSignEvnt;
	nSameSignEvnt.insert("NotSameSign");
	RegisterEvent("NotSameSign", nSameSignEvnt, "Stabilizer and distance from desired tension have different sign");
	
	set<string> tooMuchEnergyEvnt;
	tooMuchEnergyEvnt.insert("TooMuchEnergy");
	RegisterEvent("TooMuchEnergy", tooMuchEnergyEvnt, "Stabilizer is pouring too much energy");
	
	set<string> goToNormal1Evnt;
	goToNormal1Evnt.insert("SmallStabilizer");
	goToNormal1Evnt.insert("SameSign");
	RegisterEvent("GoToNormal1", goToNormal1Evnt, "Reset is over. Go to sigma_1");
	
	set<string> goToNormal2Evnt;
	goToNormal2Evnt.insert("SmallStabilizer");
	goToNormal2Evnt.insert("NotSameSign");
	RegisterEvent("GoToNormal2", goToNormal2Evnt, "Reset is over. Go to sigma_2");
	
	
	// ============== REGISTER TRANSITIONS ==================
	
	set<string> tr1;
	tr1.insert("NotSameSign");
	AddTransition("sigma_1", "sigma_2", tr1);
	
	set<string> tr2;
	tr2.insert("SameSign");
	AddTransition("sigma_2", "sigma_1", tr2);
	
	set<string> tr3;
	tr3.insert("TooMuchEnergy");
	AddTransition("sigma_1", "reset", tr3);
	AddTransition("sigma_2", "reset", tr3);
	
	set<string> tr4;
	tr4.insert("GoToNormal1");
	AddTransition("reset", "sigma_1", tr4);
	
	set<string> tr5;
	tr5.insert("GoToNormal2");
	AddTransition("reset", "sigma_2", tr5);
	
}

