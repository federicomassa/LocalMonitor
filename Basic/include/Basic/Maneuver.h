#ifndef MANEUVER_H
#define MANEUVER_H

#include <set>
#include <string>
#include <fstream>

class MyLogger;

typedef std::string ManeuverName;
typedef std::set<ManeuverName> ManeuverList;

class Maneuver
{
	ManeuverName man;
public:
	Maneuver();
	Maneuver(const ManeuverName&);
	Maneuver& operator=(const Maneuver&);
	Maneuver& operator=(const ManeuverName&);
	
	bool operator==(const ManeuverName& ) const;
	bool operator==(const Maneuver&) const;
	bool operator!=(const ManeuverName&) const;
	bool operator!=(const Maneuver&) const;
	void SetManeuver(const ManeuverName&);
	const std::string& GetName() const;
	friend MyLogger &operator<< ( MyLogger &, const Maneuver & );
	friend std::ostream &operator<< ( std::ostream &, const Maneuver & );
};

MyLogger &operator<< ( MyLogger &, const Maneuver & );
std::ostream &operator<< ( std::ostream &, const Maneuver & );

#endif
