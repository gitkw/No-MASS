from NoMASS import NoMASS

from os.path import expanduser
home = expanduser("~")

nomass = NoMASS()

nomass.runLocation = "../FMU/build/Simulation/"
nomass.locationOfNoMASS = "../FMU/build/"
nomass.configurationDirectory = home +"/git/No-MASSDSM/Configuration/WinterWeek/"

nomass.resultsLocation = "../FMU/build/Results/"
nomass.printInput = True
nomass.numberOfSimulations = 101
nomass.learnUpToSimulation = 100
nomass.simulate()
nomass.deleteLearningData()
