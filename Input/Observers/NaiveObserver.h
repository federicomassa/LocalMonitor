#ifndef NAIVE_OBSERVER_H
#define NAIVE_OBSERVER_H

#include "Observer/Observer.h"
#include <string>

class NaiveObserver : public Observer
{
public:
	NaiveObserver(const std::string& name);
	void Run() override;
};

#endif
