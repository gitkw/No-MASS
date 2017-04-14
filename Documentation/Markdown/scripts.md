# Scripts For NoMASS                        {#scripts}

There is a NoMASS.py script that implements an interface for running No-MASS as a standalone program.
The NoMASS class can be called like so.

~~~~
from NoMASS import NoMASS
nomass = NoMASS()

nomass.runLocation = "../FMU/build/Simulation/"
nomass.locationOfNoMASS = "../FMU/build/"
nomass.configurationDirectory = home +"/Dropbox/DSM_ABM/Simulations/Configurations/WinterWeek/"
nomass.resultsLocation = "../FMU/build/Results/"
nomass.printInput = True
nomass.numberOfSimulations = 1
nomass.learnUpToSimulation = 0
nomass.simulate()
nomass.deleteLearningData()
~~~~

## runLocation

NoMASS runs stochastically and often runs over multiple replicates.
