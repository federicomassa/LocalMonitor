/*
 * Base class for simulator viewers
 * */

#ifndef SIMULATORVIEWER_H
#define SIMULATORVIEWER_H

#include <string>

class SimulatorConfiguration;
class SimulAgent;

class SimulatorViewer
{
	SimulatorViewer* object;
	const SimulatorConfiguration& conf;
	std::string outputFileName;
protected:
	const SimulatorConfiguration& GetSimulatorConfiguration() const;
public:
	SimulatorViewer(const SimulatorConfiguration&);
	virtual ~SimulatorViewer();
	void Initialize(int&, char**);
	void SetOutputFileName(const std::string&);
	virtual void DrawEnvironment(); 
};

#endif
