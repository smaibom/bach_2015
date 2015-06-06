"""
Demo of a simple plot with a custom dashed line.

A Line object's ``set_dashes`` method allows you to specify dashes with
a series of on/off lengths (in points).
"""
import numpy as np
import matplotlib.pyplot as plt

#x = np.linspace(0, 22, 22)

grosses = [12550926,73555446,72870791,72870791,133865224,134413800,134413800,255427788,255427788,256276643,927802690]
labels = ['0 0 0', '0 0 1', '0 1 0', '1 0 0',
'0 0 2', '0 2 0', '2 0 0 ',
'1 0 1', '0 1 1', '1 1 0',
'1 1 1']

fig = plt.figure()
fig.subplots_adjust(bottom=0.2)          # Remark 1
ax = fig.add_subplot(111)
ax.ticklabel_format(style='plain')       # Remark 2
ax.set_xticks(range(len(labels)))
ax.set_xticklabels(labels, rotation=80)
ax.bar(range(len(grosses)), grosses)
plt.xlabel('Mismatches, deletions and insertions')
plt.ylabel('Matching states created')
plt.show()

#savefig('barchart.png', dpi=500)

#0           =  12550926     0    hits
#1ins        =  73555446     0    hits
#1del        =  72870791     0    hits
#1mut        =  72870791     0    hits
#2ins        = 133865224     2    hits
#2del        = 134413800     117  hits
#2mut        = 134413800     117  hits
#1ins 1mut   = 255427788     41   hits
#1ins 1del   = 255427788     41   hits
#1del 1mut   = 256276643     215  hits
#1 1 1       = 927802690     3138 hits