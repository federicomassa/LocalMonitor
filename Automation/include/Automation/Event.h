#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <set>

class SubEvent;

class Event
{
	std::string name;
	std::string description;
	std::set<const SubEvent*> subEvents;
	
public:
	Event(const std::string& name, const std::set<const SubEvent*>& listOfSubevents, const std::string& description = "");
	
	Event(const Event&);
	
	// Used to create sets. NB two events are equal if they have the same name
	bool operator<(const Event&) const;
	bool operator==(const Event&) const;
	
	const std::string& GetName() const;
	
};

#endif
