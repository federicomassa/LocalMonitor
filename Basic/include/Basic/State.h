#ifndef STATE_H
#define STATE_H

#include <utility>
#include <vector>
#include <string>

class Logger;

typedef std::pair<std::string, double> StatePair;
typedef std::vector<StatePair> StatePairVector;

class State
{
    friend class SimulatorConfiguration;

    StatePairVector state;

public:
    State ( const StatePairVector & );
    State();
	int Size() const;
	const StatePairVector& GetStateVector() const;
    friend Logger &operator<< ( Logger &, const State & );
	bool operator==(const State&) const;
	bool operator!=(const State&) const;
	bool operator<(const State&) const;
	bool operator<=(const State&) const;
	bool operator>(const State&) const;
	bool operator>=(const State&) const;
	State& operator=(const State&);
};

typedef std::vector<State> StateVector;

Logger &operator<< ( Logger &, const State & );

void CheckConsistency(const std::string&, const State&, const State&);

#endif
