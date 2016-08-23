
import sys, os, stat
from shutil import copyfile, copytree, rmtree
import glob
import subprocess
import random

import xml.etree.ElementTree as ET

class NoMASS(object):

    def __init__(self):
        self.runLocation = ""
        self.simulationLocation = "simulation"
        self.locationOfNoMASS = ""
        self.NoMASSstr = "NoMASS"
        self.configurationDirectory = ""
        self.simulationFile = "SimulationConfig.xml"
        self.activityFile = "Activity.xml"
        self.largeApplianceFile = "AppliancesLarge.xml"
        self.PVFile = "PVBowler2013_365.csv"
        self.smallApplianceFolder = "SmallAppliances"
        self.resultsLocation = "Results"
        self.outFile = "NoMASS.out"
        self.appLearningFile = '*.dat'
        self.numberOfSimulations = 1
        self.learnUpToSimulation = 0
        self.clean = True
        self.seed = -1
        self.printInput = False

    def deleteLearningData(self):
        ll = self.learnLoc()
        if os.path.exists(ll):
            rmtree(ll)

    def printConfiguration(self):
        print "Run location: {}".format(self.runLocation)
        print "locationOfNoMASS: {}".format(self.locationOfNoMASS)
        print "configurationDirectory: {}".format(self.configurationDirectory)
        print "resultsLocation: {}".format(self.resultsLocation)
        print "printInput: {}".format(self.printInput)
        print "numberOfSimulations: {}".format(self.numberOfSimulations)
        print "learnUpToSimulation: {}".format(self.learnUpToSimulation)
        con = self.configurationDirectory
        tree = ET.parse(con + self.simulationFile)
        root = tree.getroot()
        for buildings in root.findall('buildings'):
            for building in buildings.findall('building'):
                for agents in building.findall('agents'):
                    agentcount = 0;
                    for agent in agents.findall('agent'):
                        agentcount = agentcount +1
                    print "Number of agents {}".format(agentcount)
                for apps in building.findall('Appliances'):
                    print "Number of large appliances {}".format(len(apps.findall('Large')))
                    print "Number of small appliances {}".format(len(apps.findall('Small')))
                    print "Number of pv appliances {}".format(len(apps.findall('pv')))
                    if apps.findall('Grid'):
                        print "Grid enabled"

    def simulate(self):
        if self.printInput:
            self.printConfiguration()
        for x in range(0, self.numberOfSimulations):
            self.copyToRunLocation(x)
            self.makeExecutable(x)
            self.run(x)
            self.copyToResultsLocation(x)
            if self.clean:
                rmtree(self.runLoc(x))

    def learning(self, x):
        return x < self.learnUpToSimulation

    def configuration(self, x):
        rl = self.runLoc(x)
        tree = ET.parse(rl + self.simulationFile)
        root = tree.getroot()
        if self.seed < 0:
            random.seed()
        else:
            random.seed(self.seed)
        root.find('seed').text = str(random.randint(1,99999))
        if self.learning(x):
            root.find('learnupdate').text = 1
            root.find('learnep').text = 0.1
        else:
            root.find('learnupdate').text = 0
            root.find('learnep').text = 0.0


    def copyToResultsLocation(self, x):
        rl = self.resultsLocation
        if not os.path.exists(rl):
            os.makedirs(rl)
        outfileStr = "NoMASS-" + str(x).zfill(5) + ".out"
        copyfile(self.runLoc(x)+self.outFile, rl+outfileStr)
        outfileStr = "SimulationConfig-" + str(x).zfill(5) + ".xml"
        copyfile(self.runLoc(x)+self.simulationFile, rl+outfileStr)
        if self.learning(x):
            for f in glob.glob(self.runLoc(x) + self.appLearningFile):
                path = os.path.dirname(f)
                filename = os.path.basename(f)
                copyfile(f, rl + filename + "." + str(x).zfill(5))
                ll = self.learnLoc()
                copyfile(f, ll + filename)

    def runLoc(self, x):
        return self.runLocation + self.simulationLocation + str(x) +  "/"

    def learnLoc(self):
        return self.runLocation +  "learningdata/"

    def copyToRunLocation(self, x):
        rl = self.runLoc(x)
        if not os.path.exists(rl):
            os.makedirs(rl)
        copyfile(self.locationOfNoMASS+self.NoMASSstr, rl+self.NoMASSstr)
        con = self.configurationDirectory
        copyfile(con + self.simulationFile, rl + self.simulationFile)
        copyfile(con + self.activityFile, rl + self.activityFile)
        copyfile(con + self.largeApplianceFile, rl + self.largeApplianceFile)
        copyfile(con + self.PVFile, rl + self.PVFile)
        if os.path.isdir(rl + self.smallApplianceFolder):
            rmtree(rl + self.smallApplianceFolder)
        copytree(con + self.smallApplianceFolder, rl + self.smallApplianceFolder)
        ll = self.learnLoc()
        if self.learning(x):
            if not os.path.exists(ll):
                os.makedirs(ll)
        if os.path.exists(ll + self.appLearningFile):
            copyfile(ll + self.appLearningFile, rl + self.appLearningFile)



    def makeExecutable(self, x):
        rl = self.runLoc(x)
        nomassexe = rl+self.NoMASSstr
        st = os.stat(nomassexe)
        os.chmod(nomassexe, st.st_mode | stat.S_IEXEC)

    def run(self, x):
        rl = self.runLoc(x)
        p = subprocess.Popen('./' + self.NoMASSstr, cwd=rl)
        p.communicate()
