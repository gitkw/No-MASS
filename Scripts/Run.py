from NoMASS import NoMASS

from os.path import expanduser
home = expanduser("~")

nomass = NoMASS()

nomass.runLocation = "../FMU/build/Simulation/"
nomass.locationOfNoMASS = "../FMU/build/"
nomass.configurationDirectory = home +"/git/No-MASSDSM/Configuration/WinterWeek/"

nomass.resultsLocation = "../FMU/build/Results/"
nomass.PVFile = "PVBowler2013_365.csv"
nomass.epsilon = 0.1
nomass.alpha = 0.1
nomass.gamma = 0.1
nomass.printInput = True
nomass.numberOfSimulations = 10
nomass.learnUpToSimulation = 5
nomass.simulate()
nomass.deleteLearningData()
