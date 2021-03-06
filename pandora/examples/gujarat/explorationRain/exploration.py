#!/usr/bin/python
import fileinput, sys, os, random

numExecutions = 1

#minimumBiomassValues = ['0.1','0.2','0.3','0.4','0.5','0.6','0.7','0.8','0.9','1.0']
minimumBiomassValues = ['0.1']

meanValues = ['0', '200', '400', '600', '800', '1000', '1200', '1400', '1600', '1800', '2000', '2200', '2400', '2600', '2800', '3000', '3200', '3400', '3600', '3800', '4000', '4200', '4400','4600','4800','5000','5200','5400','5600','5800','6000','6200','6400','6600','6800','7000']
stdDevValues = ['1885']

xmlTemplate = 'templates/config_template_mn.xml'
runTemplate = 'templates/bsub_template.cmd'

indexKey = 'INDEX'
initialDirKey = 'INITIALDIR'
numExecutionKey = 'NUMEXEC'
minimumBiomassKey = 'BIOMASS_MINIMUM'
meanKey = 'MEAN'
stdDevKey = 'STDDEV'
climateKey = 'CLIMATESEED'

def replaceKey( fileName, key, value ):
	for line in fileinput.FileInput(fileName,inplace=1):
		if key in line:
			line = line.replace(key, value)
		sys.stdout.write(line)
	fileinput.close()

os.system('rm -rf mean/')
os.system('mkdir mean')

index = 0
print 'creating test workbench'

random.seed()

for numExecution in range(0,numExecutions):
	# each n.execution  has the same seed in all parameter space
	randomValue = str(random.randint(0,10000000))	
	for minimumBiomass in minimumBiomassValues:
		for mean in meanValues :
			for stddev in stdDevValues:
				print 'creating gujarat instance: ' + str(index) + ' for minimum biomass: ' + minimumBiomass + ' mean: ' + mean + ' stddev: ' + stddev + ' and execution: ' + str(numExecution)
				dirName = 'mean/results_biomin'+minimumBiomass+'_mean'+mean+'_stddev'+stddev+'_ex'+str(numExecution)
				os.system('mkdir '+dirName)
				configName = dirName + '/config.xml'			
				os.system('cp '+xmlTemplate+' '+configName)
				replaceKey(configName, minimumBiomassKey, minimumBiomass)
				replaceKey(configName, meanKey, mean)
				replaceKey(configName, stdDevKey, stddev)
				replaceKey(configName, numExecutionKey, str(numExecution))
				replaceKey(configName, climateKey, randomValue)

				runName = dirName+'/bsub_gujarat.cmd'
				os.system('cp '+runTemplate+' '+runName)
				replaceKey(runName, indexKey, str(index))
				replaceKey(runName, initialDirKey, dirName)
				index += 1

print 'workbench done, submitting tasks'
index = 0
for minimumBiomass in minimumBiomassValues:	
	for mean in meanValues :
		for stddev in stdDevValues:
			for numExecution in range(0,numExecutions):
				print 'submitting gujarat instance: ' + str(index) + ' with min biomass: ' + minimumBiomass + ' mean: ' + mean + ' stddev: ' + stddev + ' and execution: ' + str(numExecution)
				dirName = 'mean/results_biomin'+minimumBiomass+'_mean'+mean+'_stddev'+stddev+'_ex'+str(numExecution)
				os.system('bsub < '+dirName+'/bsub_gujarat.cmd')
				index += 1

