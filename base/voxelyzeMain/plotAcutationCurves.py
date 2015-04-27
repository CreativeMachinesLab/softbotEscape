#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np

f = open("ExampleActuationFile.txt")
a = 0
for line in f:
	if line.startswith("T"):
		continue
	a+=1
b = len(line.split("\t"))

data = np.zeros((a,b))

f = open("ExampleActuationFile.txt")
j=0
for line in f:
	if line.startswith("T"):
		continue
	splitLine = line.strip().split("\t")
	for i in range(len(splitLine)):
		data[j,i] = splitLine[i]
	j+=1


plt.plot(data[:,0],data[:,1], label="Material 1 (Light Blue)", alpha = 0.8, color='c')
plt.plot(data[:,0],data[:,2], label="Material 2 (Dark Blue)", alpha = 0.8, color = 'b')
plt.plot(data[:,0],data[:,3], label="Material 3 (Red)", alpha = 0.8, color='r')
plt.plot(data[:,0],data[:,4], label="Material 4 (Green)", alpha = 0.8, color='g')
plt.legend(loc=0)
plt.ylim([0.5,1.5])
plt.xlabel("time (seconds)")
plt.ylabel("size (relative to orginal)")
plt.show()