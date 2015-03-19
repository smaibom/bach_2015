def insertion(regexp):
	#Base cases
	if len(regexp) == 1: 
		return '.'
	if len(regexp) == 2:
		string = '.'+regexp[1]+'|'+regexp[0]+'.'
		return string

	string = '.' + regexp[1:] + '|' + regexp[0] + '(' + insertion(regexp[1:]) + ')'
	return string

def insertion2(regexp):
	s1 = regexp.split('|',1)
	s2 = s1[0].split('.',1)
	while(s2[0] == '('):
		s2 = s2[1].split('.',1)
	if len(s2[1]) == 1:	
		return s2[0]+"..)"
	if len(s2[0]) == 0:
		string = '.' + '(' + insertion(s2[1]) + ')' + '|' + insertion2(s1[1])
	else:
		string = s2[0] + '.(' + insertion(s2[1]) + ')|' + insertion2(s1[1])  + ')'
	return string


def inserthelp(regexp):
	splitstring = regexp.split('|',1)
	string = ".(" + insertion(splitstring[0][1:]) + ')|' 
	string = string + insertion2(splitstring[1])

	return string

if __name__ == '__main__':
	a = "abcde"
	a = insertion(a)
	a = insertion2(a)
	print insertion2(a)
