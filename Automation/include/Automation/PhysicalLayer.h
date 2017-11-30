#ifndef PHYSICAL_LAYER_H
#define PHYSICAL_LAYER_H

#include "Basic/Agent.h"
#include "Basic/Control.h"
#include "DynamicModel.h"

#include <string>

// TODO Written here because at present time the right place does not exist :D
// Check that world conversion function instantiates each world agent variable
class PhysicalLayer
{
	const double& simulTimeStep;
	DynamicModel dynamicModel;
public:
    PhysicalLayer ( const double &simulDeltaT );
	PhysicalLayer (const PhysicalLayer& );
	PhysicalLayer& operator=(const PhysicalLayer&);
	State GetNextState(const State&, const Control&) const;
	const double& GetSimulationTimeStep() const;
    void SetDynamicModel ( const DynamicModel& );
	const DynamicModel& GetDynamicModel() const;
};

#endif
