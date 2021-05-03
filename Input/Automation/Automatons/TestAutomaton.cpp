#include "TestAutomaton.h"
#include "Basic/Agent.h"
#include "Utility/EnvironmentParameters.h"
#include "Utility/Math.h"

#include <map>
#include <set>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

bool tooFast(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	if (self("v") > 25.0)
		return true;
	
	return false;
}

bool tooSlow(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	if (self("v") < 15.0)
		return true;
	
	return false;
}

bool minLane(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	double yc = self("yb")/2.0 + self("yf")/2.0;
	double laneWidth = env("lane_width");
	
	if (yc > 0 && yc < laneWidth)
		return true;
	
	return false;
}

bool targetLane(const Agent& self, const EnvironmentParameters& env, const Properties& automatonProperties)
{
	double yc = self("yb")/2.0 + self("yf")/2.0;
	double laneWidth = env("lane_width");
	
	double target = Utility::ToDouble(automatonProperties("targetLaneBegin"));
	
	// center of vehicle should be over the line with a 10% margin
	if (yc > target + 0.1*laneWidth && yc < (target + laneWidth))
		return true;
	
	return false;
}

TestAutomaton::TestAutomaton(const std::string& className) : Automaton(className)
{
	oldManeuver = Maneuver("UNKNOWN");
}


void TestAutomaton::DefineRules()
{
	//RegisterSubEvent("TooFast", tooFast, SubEvent::SINGLE, "Vehicle is moving too fast");
	//RegisterSubEvent("TooSlow", tooSlow, SubEvent::SINGLE, "Vehicle is moving too slowly");
	
	RegisterSubEvent("MinLane", minLane, SubEvent::SINGLE, "Vehicle is in first lane");
	RegisterSubEvent("TargetLane", targetLane, SubEvent::SINGLE, "Vehicle has reached target lane");
	
	set<string> subs1;
	subs1.insert("MinLane");
	RegisterEvent("MinLane", subs1, "Vehicle is in first lane");
	
	set<string> ev1;
	ev1.insert("MinLane");
	//AddTransition("FAST", "LEFT", ev1);
	
	set<string> subs2;
	subs2.insert("TargetLane");
	RegisterEvent("TargetLane", subs2, "Vehicle has reached target lane");
	
	set<string> ev2;
	ev2.insert("TargetLane");
	
	
	AddTransition("LEFT", "FAST", ev2);
}

void TestAutomaton::PreEvolve()
{
	oldManeuver = GetManeuver();
}

void TestAutomaton::PostEvolve()
{
	// Detected transition to left
	if (oldManeuver != "LEFT" && GetManeuver() == "LEFT")
	{
		const double laneWidth = GetEnvironmentTrajectory().begin().value()("lane_width");
		const double currentYf = GetSelfTrajectory().begin().value()("yf");
		const double currentYb = GetSelfTrajectory().begin().value()("yb");
		
		const double currentY = 1.0/2.0*(currentYf + currentYb);
		
		SetProperty("targetLaneBegin", Utility::ToString(std::floor(currentY/laneWidth)*laneWidth + laneWidth));
	}
	
	if (oldManeuver == "LEFT" && GetManeuver() != "LEFT")
	{
		UnsetProperty("targetLaneBegin");
	}
}



