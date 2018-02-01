#include "Maneuver.h"
#include "Utility/MyLogger.h"
#include <string>

using namespace std;

Maneuver::Maneuver() : man("")
{}

Maneuver::Maneuver(const ManeuverName& name) : man(name)
{}

Maneuver& Maneuver::operator=(const Maneuver& m)
{
	man = m.man;
	
	return *this;
}

Maneuver& Maneuver::operator=(const ManeuverName& name)
{
	man = name;
	
	return *this;
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

void Maneuver::SetManeuver(const ManeuverName& manName)
{
	man = manName;
}

MyLogger &operator<< ( MyLogger & os, const Maneuver & man)
{
	os << "Maneuver : " << man.man << os.EndL();
	
	return os;
}

ostream& operator<< ( ostream & os, const Maneuver & man)
{
	os << "Maneuver : " << std::string(man.man) << endl;
	
	return os;
}


const std::string& Maneuver::GetName() const
{
	return man;
}
