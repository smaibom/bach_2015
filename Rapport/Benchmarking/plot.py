"""
Demo of a simple plot with a custom dashed line.

A Line object's ``set_dashes`` method allows you to specify dashes with
a series of on/off lengths (in points).
"""
import numpy as np
import matplotlib.pyplot as plt

labels = ['chr1', 'chr2', 'chr3', 'chr4', 'chr5', 'chr6', 'chr7', 'chr8', 'chr9', 'chr10', 'chr11', 'chr12', 'chr13', 'chr14', 'chr15', 'chr16', 'chr17', 'chr18', 'chr19', 'chr20', 'chr21', 'chr22']
x = np.linspace(0, 22, 22)


plt.figure('data sizes')
lst = [246.284, 242.003 , 198.723 , 190.526 , 180.152 , 170.233 , 158.202 , 145.704 , 139.726 , 134.846 , 133.928, 131.833 , 113.698 , 105.954 , 99.947 , 88.481,  78.468 , 75.820 , 63.563 , 62.193 , 46.761 , 49.498 ]
plt.xticks(x, labels, rotation='vertical')
plt.plot(x, lst, 'black')
plt.ylabel('Size in megabytes')
plt.show()

plt.figure('0 Missmatches')
# C
c0 = [14.25,13.964,11.338,10.908,10.454,9.649,9.165,8.434,7.83,7.696,7.763,7.627,6.508,6.032,5.595,4.976,4.583,4.381,3.559,3.628,2.586,2.851]
# SMF
sfm0 = [4.465,4.574,3.427,3.385,3.193,2.929,2.79,2.611,2.403,2.365,2.376,2.339,1.958,1.813,1.713,1.527,1.391,1.36,1.093,1.115,0.785,0.815]
# TRE
tre0 = [44.259,43.216,35.323,34.256,32.434,30.399,28.641,26.354,25.063,24.35,24.022,23.576,20.322,18.861,17.908,15.767,14.141,13.568,11.254,11.214,8.295,8.76]
plt.plot(x, c0, 'b')
l1, = plt.plot(x, c0, '*', linewidth=2)
plt.plot(x, sfm0, color='orange')
l2, = plt.plot(x, sfm0, '-', color='orange',  linewidth=2)
plt.plot(x, tre0, 'r')
plt.xticks(x, labels, rotation='vertical')
l3, = plt.plot(x, tre0, 'o', color='r',  linewidth=2)
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()


plt.figure('1 Insertion')
# C
i21 = [16.005,16.339,12.666,12.598,10.826,10.111,9.416,8.612,7.749,7.753,7.777,7.525,6.358,5.929,5.735,4.952,4.511,4.389,3.499,3.53,2.581,2.681]
# SMF
i22 = [9.782,10.362,8.396,7.904,7.856,7.444,6.96,5.792,5.398,5.37,6.584,5.59,4.696,4.354,4.14,3.686,3.376,3.256,2.638,2.658, 1.9, 1.992]
# TRE
i23 = [50.413,48.526,40.211,38.548,35.259,32.631,30.979,27.891,26.071,25.468,25.384,24.713,21.691,19.583,18.745,16.522,14.838,14.414,11.616,11.689,8.685,9.15]
plt.plot(x, i21, 'b')
l1, = plt.plot(x, i21, '*', linewidth=2)
plt.plot(x, i22, color='orange')
l2, = plt.plot(x, i22, '-', color='orange',  linewidth=2)
plt.plot(x, i23, 'r')
plt.xticks(x, labels, rotation='vertical')
l3, = plt.plot(x, i23, 'o', color='r',  linewidth=2)
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()

plt.figure('2 Insertions')
# C
i21 = [25.438,26.681,24.798,25.003,21.724,19.603,16.789,17.281,16.246,16.167,16.111,15.715,13.221,11.871,10.971,10.67,9.843,9.41,7.735,7.797,5.378,5.7049]
plt.plot(x, i21, 'b')
l1, = plt.plot(x, i21, '*', linewidth=2)
# SMF
i22 = [12.59,12.366,10.105,9.722,9.188,8.708,8.054,7.444,6.888,6.906,6.843,6.722,5.547,5.167,4.851,4.407,4.038,3.914,3.153,3.197,2.269,2.35]
plt.plot(x, i22, 'r')
l2, = plt.plot(x, i22, '-', color='orange',  linewidth=2)
#TRE
i23 = [56.287,55.871,45.486,44.217,41.219,39.462,36.182,33.559,31.695,30.977,30.623,30.238,26.082,24.199,22.687,20.128,18.177,17.596,14.316,14.311,10.523,11.145]
plt.plot(x, i23, 'r')
plt.xticks(x, labels, rotation='vertical')
l3, = plt.plot(x, i23, 'o', color='r',  linewidth=2)
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()

plt.figure('3 Insertions')
# 3 inserts
tre3 = [56.111,55.847,45.903,43.669,41.681,39.435,36.478,33.915,31.733,31.205,30.908,30.193,26.05,24.138,22.59,20.334,18.072,17.88,14.33,14.332,10.518,11.02]
smf3 = [14.109,14.238,11.695,11.199,10.582,10.004,9.28,8.522,7.863,7.932,7.825,7.725,6.367,5.897,5.516,4.994,4.579,4.413,3.549,3.593,2.515,2.609]
c3 = [37.79,44.205,35.391,32.167,26.342,26.055,23.965,22.398,20.713,20.4,20.817,20.129,16.819,15.568,14.525,13.139,11.996,11.65,9.413,9.516,6.798,7.096]
plt.plot(x, c3, 'b')
l1, = plt.plot(x, c3, '*', linewidth=2)
plt.plot(x, smf3, color='orange')
l2, = plt.plot(x, smf3, '-', color='orange',  linewidth=2)
plt.plot(x, tre3, 'r')
plt.xticks(x, labels, rotation='vertical')
l3, = plt.plot(x, tre3, 'o', color='r',  linewidth=2)
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()


plt.figure('1 Insertion')
# C
c111 = [282.512,281.902,224.1122222,220.995,207.488,196.406,182.2144444,169.439,155.364,155.329,154.54,151.8966667,125.951,117.404,109.535,101.293,92.471,87.63,72.451,71.714,50.843,52.789]
# SMF
sfm111 = [26.342,24.69,19.659,19.331,18.414,17.46,16.043,14.96,13.742,13.854,13.795,13.607,11.056,10.362,9.737,8.989,8.343,7.958,6.481,6.546,4.484444444,4.71]
# TRE
tre111 = [70.765,69.637,55.136,54.002,51.479,49.065,45.301,41.43,39.263,39.111,38.398,38.01,32.462,29.395,27.915,24.807,23.058,22.059,17.435,17.9,12.884,13.549]

plt.plot(x, c111, 'b')
l1, = plt.plot(x, c111, '*', linewidth=2)
plt.plot(x, sfm111, color='orange')
l2, = plt.plot(x, sfm111, '-', color='orange',  linewidth=2)
plt.plot(x, tre111, 'r')
plt.xticks(x, labels, rotation='vertical')
l3, = plt.plot(x, tre111, 'o', color='r',  linewidth=2)
plt.ylabel('Time in Seconds')
plt.legend( (l1, l2, l3),('Implementation', 'scan_for_matches', 'TRE'), loc='upper right', shadow=True)
plt.show()