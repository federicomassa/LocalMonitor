#ifndef NAIVE_OBSERVER_H
#define NAIVE_OBSERVER_H

#include "Observer/Observer.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Controller.h"
#include "Automation/Automaton.h"
#include "json.hpp"
#include <string>

class NaiveObserver : public Observer
{
	PhysicalLayer pLayer;
	Automaton* automaton;
	Controller* controller;
	
	void ReadDynamicModel(const nlohmann::json&);
	void ReadControlModel(const nlohmann::json&);
public:
	NaiveObserver(const std::string& name);
	~NaiveObserver();
	void Run() override;
	void Configure(const nlohmann::json&) override;
};

#endif
