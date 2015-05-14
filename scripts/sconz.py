import os
import sys

def AddLibrary(env,incpath,libpath,libname):
	env.Append(CPPPATH = incpath)
	env.Append(LIBPATH = libpath)
	env.Append(LIBS = libname)

def MegaMerge(*args):
	ret = []
	for i in args:
		if isinstance(i,list):
			ret += i
		elif isinstance(i,str):
			ret.append(i)
	return ret

def JoinPath(*args):
	return os.path.join(args)