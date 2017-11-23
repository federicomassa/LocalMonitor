#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <map>

class Logger;

typedef std::string StateName;
typedef std::map<StateName, double> StateMap;
typedef StateMap::iterator StateIterator;
typedef StateMap::const_iterator StateConstIterator;

class DynamicModel;

class State
{
    friend class SimulatorConfiguration;
	friend class StateCell;
	friend class Environment;
	
    StateMap state;
	const StateMap &GetStateMap() const;
	double &operator[] ( const StateName & );

public:
    explicit State ( const StateMap & );
	State (const State&);
    State();
    int Size() const;
    friend Logger &operator<< ( Logger &, const State & );
    bool operator== ( const State & ) const;
    bool operator!= ( const State & ) const;
    State &operator= ( const State & );
	State operator*(const double&) const;
	State operator+(const State&) const;
	const State& operator+=(const State&);
	StateMap::iterator begin();
	StateMap::iterator end();
	StateMap::const_iterator begin() const;
	StateMap::const_iterator end() const;
	StateMap::const_iterator find(const StateName &) const;
	const double& at ( const std::string & ) const;
    double &at ( const std::string & );
	const double& operator() ( const std::string & ) const;
    double & operator() ( const std::string & );

	static State GenerateStateOfType( const State & );
	static State GenerateStateOfType( const DynamicModel & );

    friend void CheckConsistency ( const std::string &, const State &, const State & );
};

typedef std::vector<State> StateVector;

Logger &operator<< ( Logger &, const State & );

void CheckConsistency ( const std::string &, const State &, const State & );

#endif
