#include "Control.h"

#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

Control::Control(const ControlMap &u)
{
    control = u;
}

Control::Control()
{
}

Control::Control(const Control& u)
{
	control = u.GetControlMap();
}

MyLogger &operator<<(MyLogger &os, const Control &u)
{
    os << "control: " << os.EndL(MyLogger::INC);

    os << u.control << os.EndL(MyLogger::DEC);

    return os;
}

int Control::Size() const
{
    return control.size();
}

const Control::ControlMap &Control::GetControlMap() const
{
    return control;
}

bool Control::operator==(const Control &u) const
{
    CheckConsistency("Control::operator== or !=", *this, u);

    for (Control::const_iterator var = control.begin(); var != control.end(); var++)
        if (var->second != u.control.at(var->first)) {
            return false;
        }

    return true;
}

bool Control::operator!=(const Control &u) const
{
    return !(*this == u);
}


Control &Control::operator=(const Control &u)
{
    control = u.control;

    return *this;
}

const double &Control::at(const ControlName &varName) const
{
    return control.at(varName);
}

double &Control::at(const ControlName &varName)
{
    return control.at(varName);
}

const double &Control::operator()(const ControlName &varName) const
{
    return control.at(varName);
}

double &Control::operator()(const ControlName &varName)
{
    return control.at(varName);
}

double &Control::operator[](const ControlName &varName)
{
    return control[varName];
}

Control::iterator Control::begin()
{
	return control.begin();
}

Control::iterator Control::end()
{
	return control.end();
}

Control::const_iterator Control::begin() const
{
	return control.cbegin();
}

Control::const_iterator Control::end() const
{
	return control.cend();
}

Control::const_iterator Control::find(const ControlName & name) const
{
	return control.find(name);
}

Control Control::GenerateControlOfType(const Control & q)
{
	Control newControl;
	
	for (ControlMap::const_iterator var = q.control.begin(); var != q.control.end(); var++)
		newControl[var->first] = 0.0;
	
	return newControl;
}


void CheckConsistency(const string &className, const Control &q1, const Control &q2)
{
    string errorString = "Controls should be of same type";

    Require(q1.Size() == q2.Size(), className, errorString);

	
    for (Control::const_iterator var = q1.GetControlMap().begin(); var != q1.GetControlMap().end(); var++)
        Require(q2.GetControlMap().find(var->first) != q2.GetControlMap().end(),
                className, errorString);
		
	// Check also the viceversa, but avoiding repetitions
	for (Control::const_iterator var = q2.GetControlMap().begin(); var != q2.GetControlMap().end(); var++)
		if (q1.GetControlMap().find(var->first) == q1.GetControlMap().end())
			Require(q1.GetControlMap().find(var->first) != q1.GetControlMap().end(),
                className, errorString);
	
}


