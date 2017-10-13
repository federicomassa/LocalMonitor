#include "State.h"
#include "Utility/Logger.h"
#include "Utility/LogFunctions.h"

#include <string>

using namespace std;
using namespace LogFunctions;

State::State(const StatePairVector &s)
{
    state = s;
}

State::State()
{
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

const StatePairVector& State::GetStateVector() const
{
	return state;
}

bool State::operator==(const State& q) const
{
	CheckConsistency("State::operator== or !=", *this, q);
	
	for (int i = 0; i < q.Size(); i++)
		if (state.at(i).second != q.state.at(i).second)
			return false;
		
	return true;
}

bool State::operator!=(const State& q) const
{
	return !(*this == q);
}

bool State::operator<(const State& q) const
{
	CheckConsistency("State::operator< or >=", *this, q);
	
	for (int i = 0; i < q.Size(); i++)
		if (state.at(i).second >= q.state.at(i).second)
			return false;
		
	return true;
}

bool State::operator<=(const State& q) const
{
	CheckConsistency("State::operator<= or >", *this, q);
	
	for (int i = 0; i < q.Size(); i++)
		if (state.at(i).second > q.state.at(i).second)
			return false;
		
	return true;	
}

bool State::operator>(const State& q) const
{
	return !(*this <= q);
}

bool State::operator>=(const State& q) const
{
	return !(*this < q);
}

State& State::operator=(const State& q)
{
	state = q.state;
	
	return *this;
}

void CheckConsistency(const string& className, const State& q1, const State& q2)
{
	string errorString = "States should be of same type";
	
	Require(q1.Size() == q2.Size(), className, errorString);
	for (int i = 0; i < q1.Size(); i++)
		Require(q1.GetStateVector().at(i).first == q2.GetStateVector().at(i).first,
				"StateCell::StateCell", errorString);
}
