#ifndef MANEUVER_H
#define MANEUVER_H

#include <set>
#include <string>

class Logger;

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
	bool SetManeuver(const ManeuverName&);
	const std::string GetName() const;
	friend Logger &operator<< ( Logger &, const Maneuver & );
};

Logger &operator<< ( Logger &, const Maneuver & );


#endif
