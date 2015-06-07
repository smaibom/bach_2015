import regex
with open("../Data/chr22.txt") as myfile:
    sequence = myfile.read().repalce('\n','').upper()
p = regex.compile("(?:TGCAAGCGTTAAT){s<=1,d<=0,i<=1}")
print len(p.findall(sequence, overlapped=True))