import subprocess
from shutil import copyfile, copytree, rmtree, move
import random
from time import sleep
import sys, os
import xml.etree.ElementTree as ET
from configuration import *


runLocation = sys.argv[2]
run = int(sys.argv[1])
sims = int(sys.argv[3])

print runLocation

configLocations = configurations()

currentRunSim = -1
currentRun = {}
runnableLocations = []
count = 0;
for x in configLocations:
    if x['run']:
        runnableLocations.append(x)


stop = False
bot = 0
top = 0
totalsim = sims * len(runnableLocations)
for x in runnableLocations:
    if stop:
        break
    sim = 0
    for y in x['sims']:
        bot = sims * count
        top = sims * (count +1)
        if (run <= top) and (run > bot):
            currentRun = x
            currentRunSim = sim
            stop = True
            break
        sim = sim + 1
        count = count + 1




location = runLocation +'/' + currentRun['sims'][currentRunSim] + str(run) +'/'

currentResults = resultsFolder + "/" + currentRun['name'] + "/" + currentRun['sims'][currentRunSim]
if 'outFolder' in currentRun:
    currentResults = resultsFolder + "/" + currentRun['name'] + "/" + currentRun['outFolder']

location = runLocation + '/ReadVars' + currentRun['sims'][currentRunSim] + str(run) +'/'

configLocations = outputs()

periods = ['runperiod','timestep','monthly']
periods = ['runperiod','timestep']

for j in periods:
    for i in configLocations:
        with open(location + 'myfile.rvi', 'w+') as f:
            f.write(location + 'eplusout.eso\n')
            f.write(currentResults +'/'+ j +  str(run) + '.csv\n')
            for x in i:
                f.write(x+'\n')
            f.write('0\n')

        p = subprocess.Popen([location+'ReadVarsESO', location+'myfile.rvi', j], cwd=location)
        p.communicate()


rmtree(location)
