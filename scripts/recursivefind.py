import os

def CheckExt(name,exts):
	for ext in exts:
		if name.endswith(ext):
			return True
	return False

def GetFiles(folder,exts = ("",)):
	from os.path import join, getsize
	for root, dirs, files in os.walk(folder):
		for name in files:
			if CheckExt(name,exts):
				yield root+os.sep+name

def QuotePath(path):
	return "\"" + path + "\""

import pprint
import subprocess

def DecLua():
	for each in GetFiles("../bin/lol",("luaobj",)):
		lua = open(each[:-6]+"lua","w")
		try:
			data = subprocess.check_output(
					"luadec %s"%each,
					stderr=subprocess.STDOUT
				).decode("ascii")
		except:
			try:
				data = "Decompiling Error, Disassemble instead\n"
				data += subprocess.check_output(
							"luadec %s"%each,
							stderr=subprocess.STDOUT
						).decode("ascii")
				print("Dis:",each)
			except:
				data = "Dec/Dis Supa Error"
				print("Err:",each)
		lua.write(
			data
		)
		lua.close()

def ConImg():
	for each in GetFiles("..\\bin\\lol",("png","jpg")):
		png = each[:-3]+"png"
		ret = os.system("convert %s png32:%s"%(QuotePath(each),QuotePath(png)))
		print(each,png)

if __name__ == '__main__':
	#DecLua()
	ConImg()