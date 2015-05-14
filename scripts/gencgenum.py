out = open("cg.h.lst","w")

for line in open("C://Program Files (x86)//NVIDIA Corporation//Cg//include//Cg//cg.h").readlines():
	if line.startswith("  CG_"):
		s = line.split()
		try:
			val = int(s[2][:-1])
			name = s[0]
			out.write("%s %d\n"%(name,val))

		except:
			print(s)

