#include "State.h"
#include "Automation/DynamicModel.h"
#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"

#include <string>
#include <iostream>

using namespace std;
using namespace LogFunctions;

State::State(const StateMap &s)
{
    state = s;
}

State::State()
{
}

State::State(const State& q)
{
	state = q.GetStateMap();
}

MyLogger &operator<<(MyLogger &os, const State &s)
{
    os << "state: " << os.EndL(MyLogger::INC);

    os << s.state << os.EndL(MyLogger::DEC);

    return os;
}

int State::Size() const
{
    return state.size();
}

const StateMap &State::GetStateMap() const
{
    return state;
}

bool State::operator==(const State &q) const
{
    CheckConsistency("State::operator== or !=", *this, q);

    for (StateMap::const_iterator var = state.begin(); var != state.end(); var++)
        if (var->second != q.state.at(var->first)) {
            return false;
        }

    return true;
}

bool State::operator!=(const State &q) const
{
    return !(*this == q);
}


State &State::operator=(const State &q)
{
    state = q.state;

    return *this;
}

State State::operator*(const double& k) const
{
	State result = *this;
	
	for (StateMap::const_iterator var = result.state.begin(); var != result.state.end(); var++)
	{
		result.at(var->first) = var->second*k; 
	}
	
	return result;
}

State State::operator/(const double& k) const
{
	Require(k != 0, "State::operator/", "Requested division for zero");
	
	State result = *this;
	
	for (StateMap::const_iterator var = result.state.begin(); var != result.state.end(); var++)
	{
		result.at(var->first) = var->second/k; 
	}
	
	return result;
}

State State::operator+(const State& q) const
{
	CheckConsistency("State::operator+", *this, q);
	
	State result = *this;
	
	for (StateMap::const_iterator var = result.state.begin(); var != result.state.end(); var++)
	{
		result.at(var->first) = var->second + q.at(var->first); 
	}
	
	return result;
}

State State::operator-(const State& q) const
{
	CheckConsistency("State::operator-", *this, q);
	
	State result = *this;
	
	for (StateMap::const_iterator var = result.state.begin(); var != result.state.end(); var++)
	{
		result.at(var->first) = var->second - q.at(var->first); 
	}
	
	return result;
}

const State& State::operator+=(const State& q)
{
	CheckConsistency("State::operator+", *this, q);
	
	for (StateMap::const_iterator var = state.begin(); var != state.end(); var++)
	{
		at(var->first) = var->second + q.at(var->first); 
	}
	
	return *this;
}

const State& State::operator-=(const State& q)
{
	CheckConsistency("State::operator-=", *this, q);
	
	for (StateMap::const_iterator var = state.begin(); var != state.end(); var++)
	{
		at(var->first) = var->second - q.at(var->first); 
	}
	
	return *this;
}


const double &State::at(const std::string &varName) const
{
	return state.at(varName);
}

double &State::at(const std::string &varName)
{
	return state.at(varName);
}

const double &State::operator()(const std::string &varName) const
{
    return state.at(varName);
}

double &State::operator()(const std::string &varName)
{
    return state.at(varName);
}

double &State::operator[](const StateName &varName)
{
    return state[varName];
}

void State::AddStateVariable(const std::string& varName)
{
	state[varName];
}


StateMap::iterator State::begin()
{
	return state.begin();
}

StateMap::iterator State::end()
{
	return state.end();
}

StateMap::const_iterator State::begin() const
{
	return state.cbegin();
}

StateMap::const_iterator State::end() const
{
	return state.cend();
}

StateMap::const_iterator State::find(const StateName & name) const
{
	return state.find(name);
}

State State::GenerateStateOfType(const State & q)
{
	State newState;
	
	for (StateMap::const_iterator var = q.state.begin(); var != q.state.end(); var++)
		newState[var->first] = 0.0;
	
	return newState;
}

State State::GenerateStateOfType(const DynamicModel & m)
{
	State newState;
	
	for (auto var = m.stateVars.begin(); var != m.stateVars.end(); var++)
		newState[*var] = 0.0;
	
	return newState;
}

State State::GenerateStateOfType(const set<string>& vars)
{
	State newState;
	
	for (auto var = vars.begin(); var != vars.end(); var++)
		newState[*var] = 0.0;
	
	return newState;
}

State State::GenerateStateOfType(const vector<string>& vars)
{
	State newState;
	
	for (auto var = vars.begin(); var != vars.end(); var++)
		newState[*var] = 0.0;
	
	return newState;
}


void CheckConsistency(const string &className, const State &q1, const State &q2)
{
    string errorString = "States should be of same type";

    Require(q1.Size() == q2.Size(), className, errorString);

	
    for (StateMap::const_iterator var = q1.GetStateMap().begin(); var != q1.GetStateMap().end(); var++)
        Require(q2.GetStateMap().find(var->first) != q2.GetStateMap().end(),
                className, errorString);
		
	// Check also the viceversa, but avoiding repetitions
	for (StateMap::const_iterator var = q2.GetStateMap().begin(); var != q2.GetStateMap().end(); var++)
		if (q1.GetStateMap().find(var->first) == q1.GetStateMap().end())
			Require(q1.GetStateMap().find(var->first) != q1.GetStateMap().end(),
                className, errorString);
	
}
