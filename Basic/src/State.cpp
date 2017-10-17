#include "State.h"
#include "Utility/Logger.h"
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

Logger &operator<<(Logger &os, const State &s)
{
    os << "state: " << os.EndL(Logger::INC);

    os << s.state << os.EndL(Logger::DEC);

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

void State::SetStateVar(const std::string &varName, const double &val)
{
    state.at(varName) = val;
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

bool State::operator<(const State &q) const
{
    CheckConsistency("State::operator< or >=", *this, q);

    for (StateMap::const_iterator var = state.begin(); var != state.end(); var++)
        if (var->second >= q.state.at(var->first)) {
            return false;
        }

    return true;
}

bool State::operator<=(const State &q) const
{
    CheckConsistency("State::operator<= or >", *this, q);

    for (StateMap::const_iterator var = state.begin(); var != state.end(); var++)
        if (var->second > q.state.at(var->first)) {
            return false;
        }

    return true;
}

bool State::operator>(const State &q) const
{
    return !(*this <= q);
}

bool State::operator>=(const State &q) const
{
    return !(*this < q);
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
		result.SetStateVar(var->first, var->second*k); 
	}
	
	return result;
}

State State::operator+(const State& q) const
{
	CheckConsistency("State::operator+", *this, q);
	
	State result = *this;
	
	for (StateMap::const_iterator var = result.state.begin(); var != result.state.end(); var++)
	{
		result.SetStateVar(var->first, var->second + q.at(var->first)); 
	}
	
	return result;
}

const State& State::operator+=(const State& q)
{
	CheckConsistency("State::operator+", *this, q);
	
	for (StateMap::const_iterator var = state.begin(); var != state.end(); var++)
	{
		SetStateVar(var->first, var->second + q.at(var->first)); 
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

double &State::operator[](const std::string &varName)
{
    return state[varName];
}

State State::GenerateStateOfType(const State & q)
{
	State newState;
	
	for (StateMap::const_iterator var = q.state.begin(); var != q.state.end(); var++)
		newState[var->first] = 0.0;
	
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


