file1 = open('a1Mb.txt', 'w')
a1Mb = "a"*(10**6)+"b"
file1.write(a1Mb)
file1.close()