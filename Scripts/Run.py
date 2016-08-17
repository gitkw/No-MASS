from NoMASS import NoMASS

from os.path import expanduser
home = expanduser("~")

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
