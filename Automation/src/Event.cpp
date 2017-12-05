#include "Event.h"
#include "SubEvent.h"

using namespace std;

Event::Event(const string& n, const set<const SubEvent*>& listOfSubevents, const string& descr)
{
	name = n;
	description = descr;
	subEvents = listOfSubevents;
}

Event::Event(const Event& e)
{
	name = e.name;
	description = e.description;
	subEvents = e.subEvents;
} 

const string& Event::GetName() const
{
	return name;
}

bool Event::operator<(const Event& e) const
{
	return (name < e.name);
}

bool Event::operator==(const Event& e) const
{
	bool equal = true;
	equal = equal && (name == e.name);
	
	return equal;
}



