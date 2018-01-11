#ifndef OBSERVERS_H
#define OBSERVERS_H

#include <string>

class Observer;

Observer* InstantiateObserver(const std::string& observerClassName);

#endif
