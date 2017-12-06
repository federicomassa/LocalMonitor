#include "Maneuver.h"
#include "Utility/Logger.h"
#include <string>

Maneuver::Maneuver() : man("")
{}

Maneuver::Maneuver(const ManeuverName& name) : man(name)
{}

Maneuver& Maneuver::operator=(const Maneuver& m)
{
	man = m.man;
}

Maneuver& Maneuver::operator=(const ManeuverName& name)
{
	man = name;
}

bool Maneuver::operator==(const Maneuver& m) const
{
	if (man == m.man)
		return true;
	
	return false;
}

bool Maneuver::operator==(const ManeuverName& mName) const
{
	if (man == mName)
		return true;
	
	return false;
}

bool Maneuver::operator!=(const Maneuver& m) const
{
	return !(*this == m);
}

bool Maneuver::operator!=(const ManeuverName& mName) const
{
	return !(*this == mName);
}

bool Maneuver::SetManeuver(const ManeuverName& manName)
{
	man = manName;
}

Logger &operator<< ( Logger & os, const Maneuver & man)
{
	os << "Maneuver : " << man.man << os.EndL();
}

const std::string Maneuver::GetName() const
{
	return man;
}
