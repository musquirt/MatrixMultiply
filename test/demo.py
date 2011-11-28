from subprocess import *
from string import atoi

def compareOutputMatrix(m_int, m_str):
	for i in range(0, len(m_int)):
		m_a = m_int[i]
		m_b = m_str[i].split(" ")
		m_b.remove('')
		for j in range(0, len(m_a)):
			if not (str(m_a[j]) == str(m_b[j])):
				return False
	return True

def doMatrixMath(s):
	matrices = s.splitlines()
	matrixA = []
	matrixB = []
	matrixC = []
	matrixJunk = []
	for item in matrices:
		if item == "multiplied by":
			matrixA = matrixJunk
			matrixJunk = []
		elif item == "equals":
			matrixB = matrixJunk
			matrixJunk = []
		elif ("us" in item):
			matrixC = matrixJunk
			matrixJunk = []
		else:
			matrixJunk.append(item)
	intMatA = [[]]
	for a in matrixA:
		t = a.split(" ")
		t.remove('')
		intMatA.append(t)
	intMatB = [[]]
	for ab in matrixB:
		u = ab.split(" ")
		u.remove('')
		intMatB.append(u)
	
	
	i = 0
	intMatA.remove([])
	intMatB.remove([])
	intMatC = [[]]
	
	for a in intMatA:
		tempMat = []
		for k in range(0, len(intMatB[0])):
			j = 0
			num = 0
			for item in a:
				b = intMatB[j]
				num = num + int(item) * int(b[k])
				j = j+1
			tempMat.append(num)
		intMatC.append(tempMat)
		i = i + 1
	intMatC.remove([])
	
	return compareOutputMatrix(intMatC, matrixC)

def runSpeedTest(r1, c1, r2, c2, mtype, testnum):
	print "Speed Test %i: %ix%i * %ix%i" % (testnum, r1, c1, r2, c2)
	out = check_output(["./multiply", "-a", str(r1), str(c1), "-b", str(r2), str(c2),"-t","1"])
	print "    Time using 1 thread:  ",out,
	ostr = out.split(" ")
	unithread = ostr[0]
	out = check_output(["./multiply", "-a", str(r1), str(c1), "-b", str(r2), str(c2)])
	print "    Time using 8 threads: ",out,
	ostr = out.split(" ")
	multithread = ostr[0]
	if (atoi(unithread) < atoi(multithread)):
		print "    Multi-threading slower than single threading"
	else:
		print "    Multi-threading faster than single threading"
	return

def runSanityTest(r1, c1, r2, c2, testnum):
	print "Sanity Tests: %ix%i * %ix%i" % (r1, c1, r2, c2)
	for i in range(1,9):
		print "    Sanity Test %i (%i threads): " % (i,i),
		out = check_output(["./multiply", "-a", str(r1), str(c1), "-b", str(r2), str(c2),"-t", str(i), "-rand", "-v"])
		if doMatrixMath(out):
			print "PASSED"
		else:
			print "FAILED"
	return;

runSanityTest(13, 8, 8, 16, 1)

i = 1;
n = 1;
while i < 1000:
	runSpeedTest(i, i, i, i,"ident", n)
	runSpeedTest(3*i, 4*i, 4*i, 3*i, "ident", n+1)
	runSpeedTest(7*i, 3*i, 3*i, 4*i, "ident", n+2)
	runSpeedTest(i, 2*i, 2*i, 15*i,"ident",n+3)
	i = i * 10
	n = n+4

