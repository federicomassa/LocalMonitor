#ifndef ACC_OMEGA_CONTROL_H
#define ACC_OMEGA_CONTROL_H

class State;
class Maneuver;

Control AccOmegaControl(const State& q0 ,const Maneuver& maneuver);

#endif
