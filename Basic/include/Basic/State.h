#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <map>

class Logger;

typedef std::map<std::string, double> StateMap;

class State
{
    friend class SimulatorConfiguration;

    StateMap state;

public:
	const StateMap &GetStateMap() const;
    explicit State ( const StateMap & );
	State (const State&);
    State();
    int Size() const;
    friend Logger &operator<< ( Logger &, const State & );
    bool operator== ( const State & ) const;
    bool operator!= ( const State & ) const;
    bool operator< ( const State & ) const;
    bool operator<= ( const State & ) const;
    bool operator> ( const State & ) const;
    bool operator>= ( const State & ) const;
    State &operator= ( const State & );
	State operator*(const double&) const;
	State operator+(const State&) const;
	const State& operator+=(const State&);

    void SetStateVar ( const std::string &, const double & );
    double &operator[] ( const std::string & );
    const double &at ( const std::string & ) const;
    double &at ( const std::string & );

	static State GenerateStateOfType( const State & );
    friend void CheckConsistency ( const std::string &, const State &, const State & );
};

typedef std::vector<State> StateVector;

Logger &operator<< ( Logger &, const State & );

void CheckConsistency ( const std::string &, const State &, const State & );

#endif
