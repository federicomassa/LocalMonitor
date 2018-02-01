#ifndef PHYSICAL_LAYER_H
#define PHYSICAL_LAYER_H

#include "Basic/Agent.h"
#include "Control.h"
#include "DynamicModel.h"

#include <string>

class MyLogger;

// TODO Written here because at present time the right place does not exist :D
// Check that world conversion function instantiates each world agent variable
class PhysicalLayer
{
	double simulTimeStep;
	DynamicModel dynamicModel;

	MyLogger* logger;
public:
	PhysicalLayer();
    PhysicalLayer ( const double &simulDeltaT, MyLogger* log = nullptr );
	PhysicalLayer (const PhysicalLayer& );
	PhysicalLayer& operator=(const PhysicalLayer&);
	State GetNextState(const Agent&, const AgentVector&, const Control&) const;
	const double& GetSimulationTimeStep() const;
	void SetSimulationTimeStep(const double&);
    void SetDynamicModel ( const DynamicModel& );
	const DynamicModel& GetDynamicModel() const;
};

#endif
