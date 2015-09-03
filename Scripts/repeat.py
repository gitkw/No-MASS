import subprocess
from shutil import copyfile, copytree, rmtree
import random
from time import sleep
import sys, os
import multiprocessing
from multiprocessing import Process, Lock

import xml.etree.ElementTree as ET

run = 0

sims = 1

configVariables = []
configLocations = []
configname= []
#Output:Reports, Schedules, Timestep;
#Output:Variable,*,Schedule Value,Timestep;

configVariables.append('Zone Air System Sensible Heating Energy')
configVariables.append('Zone Air System Sensible Cooling Energy')
configVariables.append('Zone People Occupant Count')
configVariables.append('Zone Mean Air Temperature')
configVariables.append('Daylighting Reference Point 1 Illuminance')
configVariables.append('Site Exterior Horizontal Sky Illuminance')
configVariables.append('Zone Exterior Windows Total Transmitted Beam Solar Radiation Rate')
configVariables.append('Window Shading Fraction')
configVariables.append('AFN Surface Venting Window or Door Opening Factor')
configVariables.append('BLOCK1:ZONE1WINDOWSTATE,Schedule Value')
configVariables.append('BLOCK1:ZONE1LIGHTSTATE,Schedule Value')
configLocations.append(configVariables)
configname.append('All')


#configLocations.append(['BLOCK1:ZONE1BLINDSTATE,Schedule Value'])
#configname.append('shadestate')

periods = ['runperiod','timestep','monthly']




def runEP(run):
    location = home + '/git/EnergyPlus' + str(run) +'/'
    if os.path.isdir(location):
        rmtree(location)

    copytree(home + '/git/EnergyPlusRunning/',location)

    xmlFile = location + 'tmp-fmus/agentFMU.fmu_FMI/SimulationConfig.xml'
    if os.path.exists(xmlFile):
        tree = ET.parse(xmlFile)
        root = tree.getroot()
        root.find('seed').text = str(run * 989)
        root.find('rlearn').text = str(1 - (run / 10))
        tree.write(xmlFile)


    p = subprocess.Popen(location+'EnergyPlus', cwd=location)
    p.communicate()

    if not os.path.exists(home + "/git/results"):
        os.makedirs(home + "/git/results")

    copyfile(location+'eplusout.eso', home + "/git/results/eplusout" + str(run)+'.eso')
    copyfile(location+'eplusout.err', home + "/git/results/eplusout" + str(run)+'.err')
    copyfile(location+'eplusout.rdd', home + "/git/results/eplusout" + str(run)+'.rdd')
    copyfile(location+'rlearning0.dat', home + "/git/EnergyPlusRunning/rlearning0.dat")
    copyfile(location+'rlearning1.dat', home + "/git/EnergyPlusRunning/rlearning1.dat")
    copyfile(location+'rlearning0.dat', home + "/git/results/rlearning0" + str(run)+'.dat')
    copyfile(location+'rlearning1.dat', home + "/git/results/rlearning1" + str(run)+'.dat')
    if os.path.exists(location+'eplusout.edd'):
        copyfile(location+'eplusout.edd', home + "/git/results/eplusout" + str(run)+'.edd')


    confignamecount = 0
    for i in configLocations:
        for j in periods:
            name = configname[confignamecount]
            name = name.replace(" ", "")

            f = open(location+'myfile.rvi','w')
            f.write(home + "/git/results/eplusout" + str(run)+'.eso\n')

            f.write(home + '/git/results/'+ name + j +  str(run) + '.csv\n') # python will convert \n to os.linesep

            for x in i:
                f.write(x+'\n') # python will convert \n to os.linesep
            f.write('0\n') # python will convert \n to os.linesep
            f.close() # you can omit in most cases as the destructor will call if
            p = subprocess.Popen([location+'ReadVarsESO', location+'myfile.rvi', j], cwd=location)
            p.communicate()
        confignamecount = confignamecount +1

    rmtree(location)

def runmulti(run):
    for proc in range(0,cpus):
        runEP(run)


home = "/home/jake"

runs = 10

runmulti(runs)
