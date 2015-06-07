"""
Demo of a simple plot with a custom dashed line.

A Line object's ``set_dashes`` method allows you to specify dashes with
a series of on/off lengths (in points).
"""
import numpy as np
import matplotlib.pyplot as plt

#x = np.linspace(0, 22, 22)

grosses = [62242364,123246883,122562228,122562228,183556657,184105105,184105105,243618278,245304681,245304681,305119181,305119181,305967853,977490276]
labels = ['0 0 0',
'0 0 1', '0 1 0', '1 0 0',
'0 0 2', '0 2 0', '2 0 0 ',
'0 0 3', '0 3 0', '3 0 0 ',
'1 0 1', '0 1 1', '1 1 0',
'1 1 1']

fig = plt.figure()
fig.subplots_adjust(bottom=0.2)          # Remark 1
ax = fig.add_subplot(111)
ax.ticklabel_format(style='plain')       # Remark 2
ax.set_xticks(range(len(labels)))
ax.set_xticklabels(labels, rotation=80)
ax.bar(range(len(grosses)), grosses)
plt.xlabel('alterations, deletions and insertions\n(In order)')
plt.ylabel('states processed')
plt.show()

#0           =  62242364     0    hits
#1ins        =  123246883     0    hits
#1del        =  122562228     0    hits
#1mut        =  122562228    0    hits
#2ins        =  183556657     3    hits
#2del        =  184105105     117  hits
#2mut        =  184105105    117  hits
#3ins        =  243618278     28   hits
#3del        =  245304681     2066 hits
#3mut        = 245304681     2066 hits
#1ins 1mut   = 305119181     41   hits
#1ins 1del   = 305119181     41   hits
#1del 1mut   = 305967853     234  hits
#1 1 1       = 977490276     3275 hits