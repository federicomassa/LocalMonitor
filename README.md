#################################################################################
Authors = Federico Massa, Adriano Fagiolini
Date = 02.10.2017
Note for devs = This is a generalized and modular version of Sim_Highway 
#################################################################################

LocalMonitor is a software designed to work in a distributed multi-agent environment
and its goal is to monitor the behaviour of an agent whose state follows a known hybrid
dynamics (finite-state-machine where each state correspond to a different time-continuous
controller). The transition among discrete states must be regulated by rules based
on logical events, that depend on the interaction among states of 'neighboring'
agents.

The software is designed to be reconfigurable by the user, that writes
a UserConfig file to describe the environment and some functions that
describe the motion protocol of the observed agent.

## Installation (via docker)

To build:
```
docker build -t localmonitor:latest .
```

To open a shell inside the docker image:
```
xhost +
docker run --network=host --env="DISPLAY" --env="QT_X11_NO_MITSHM=1" --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" -v /absolute/path/to/localmonitor/Output:/home/Output -it localmonitor:latest bash
```

And when you are done:
```
xhost -
```

Inside the shell, type:

```
build/Simulator/Simulator
```

to launch a simulation.

To configure the simulation, edit `Input/Simulator/Simulator.json`; to configure the observers, edit `Input/Observers/<ID>/Observer.json`; to change the viewer type, edit `Simulator/SET_SIMULATOR_VARIABLES.in`