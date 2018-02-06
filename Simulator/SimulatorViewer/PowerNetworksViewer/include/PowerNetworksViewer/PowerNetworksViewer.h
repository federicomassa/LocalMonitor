#ifndef POWER_NETWORKS_VIEWER_H
#define POWER_NETWORKS_VIEWER_H

#include "SimulatorViewer/SimulatorViewer.h"
#include <fstream>
#include "Utility/MyLogger.h"
#include "Utility/IMap.h"
#include <string>
#include <set>

class SimulatorConfiguration;

class PowerNetworksViewer : public SimulatorViewer
{
	IMap<std::ofstream*> output;
	IMap<MyLogger> logger;
	
	const SimulatorConfiguration* config;
	
	std::set<std::string> variables;
public:
	PowerNetworksViewer(const SimulatorConfiguration&);
	~PowerNetworksViewer();
	void DrawStaticEnvironment() override;
	void DrawDynamicEnvironment(const SimulAgentVector&, const double& currentTime) override;
};

#endif
