#ifndef PHYSICAL_LAYER_H
#define PHYSICAL_LAYER_H

#include "Basic/Agent.h"

#include <string>

class PhysicalLayer
{
    typedef void ( *KinematicsFcn ) ( State &, const State &, const Maneuver & );
	double simulTimeStep;
    KinematicsFcn kinematics;

public:
    PhysicalLayer ( const double &simulDeltaT );
	PhysicalLayer (const PhysicalLayer& );
	PhysicalLayer& operator=(const PhysicalLayer&);
	State GetNextState(const State&, const Maneuver&) const;
	const double& GetSimulationTimeStep() const;
    void SetKinematics ( KinematicsFcn kin );
};

#endif
