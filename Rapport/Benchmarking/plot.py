"""
Demo of a simple plot with a custom dashed line.

A Line object's ``set_dashes`` method allows you to specify dashes with
a series of on/off lengths (in points).
"""
import numpy as np
import matplotlib.pyplot as plt

labels = ['chr1', 'chr2', 'chr3', 'chr4', 'chr5', 'chr6', 'chr7', 'chr8', 'chr9', 'chr10', 'chr11', 'chr12', 'chr13', 'chr14', 'chr15', 'chr16', 'chr17', 'chr18', 'chr19', 'chr20', 'chr21', 'chr22']
x = np.linspace(0, 22, 22)

plt.figure('2 Insertions')
# C
i21 = [56.664, 60.535, 49.772, 49.415, 45.268, 43.216, 38.432, 36.174, 31.322, 32.349, 31.492, 32.556, 27.026, 24.059, 21.951, 19.242, 17.735, 19.884, 12.587, 14.287, 9.978, 8.955 ]
plt.plot(x, i21, 'b')
l1, = plt.plot(x, i21, '*', linewidth=2)
# SMF
i22 = [12.59,12.366,10.105,9.722,9.188,8.708,8.054,7.444,6.888,6.906,6.843,6.722,5.547,5.167,4.851,4.407,4.038,3.914,3.153,3.197,2.269,2.35]
plt.plot(x, i22, 'r')
l2, = plt.plot(x, i22, '-', color='orange',  linewidth=2)
#TRE
i23 = [56.287,55.871,45.486,44.217,41.219,39.462,36.182,33.559,31.695,30.977,30.623,30.238,26.082,24.199,22.687,20.128,18.177,17.596,14.316,14.311,10.523,11.145]
plt.plot(x, i23, 'r')
l3, = plt.plot(x, i23, 'o', color='r',  linewidth=2)

plt.xticks(x, labels, rotation='vertical')
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()

plt.figure('1 Insertion')
# C
i21 = [20.25, 20.974, 15.932,16.83,15.642,14.618,13.538,11.704,11.01,10.866,11.206,11.064,9.452,8.758,8.218,7.434,6.628,6.45,5.27,5.326,3.924,4.166]
i22 = [9.782,10.362,8.396,7.904,7.856,7.444,6.96,5.792,5.398,5.37,6.584,5.59,4.696,4.354,4.14,3.686,3.376,3.256,2.638,2.658, 1.9, 1.992]

plt.plot(x, i21, 'b')
l1, = plt.plot(x, i21, '*', linewidth=2)
# SMF
plt.plot(x, i22, color='orange')
l2, = plt.plot(x, i22, '-', color='orange',  linewidth=2)
i23 = [53.148, 54.965]
plt.plot([1,2], i23, 'r')
plt.xticks(x, labels, rotation='vertical')
x = np.linspace(0, 2, 2)
l3, = plt.plot(x, i23, 'o', color='r',  linewidth=2)
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()