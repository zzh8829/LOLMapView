import os
import sys
sys.path.append(os.path.realpath('scripts'))

import platform

import sconz
import recursivefind

env = Environment()

BINDIR = "bin"
OBJDIR = "build"
SRCDIR = "source"

TARGET = "lolz"

PLATFORM = {
	"Windows"	:"PLATFORM_WINDOWS",
	"Linux"		:"PLATFORM_LINUX",
	"Darwin"	:"PLATFORM_OSX",
	""			:"PLATFORM_UNKNOWN"
	}[platform.system()]

ARCH = platform.architecture()

STDFLAG	= "-std=gnu++11"
OPTFLAG	= "-O3"
WARNING	= ["-Wall","-Wextra"]
NOWARNING = [
	"-Wno-unused-function", "-Wno-unused-parameter","-Wno-sign-compare",
	"-Wno-trigraphs", "-Wno-unused-variable", "-Wno-unused-but-set-variable"
]

env['CCFLAGS'] = sconz.MegaMerge(STDFLAG,OPTFLAG,WARNING,NOWARNING)
env.Append(CPPDEFINES = [PLATFORM])
env.Append(CPPPATH = [SRCDIR])

if PLATFORM == "PLATFORM_WINDOWS":
	env.Tool("mingw")

	env.Append(CCFLAGS = ["-mconsole","-static-libgcc"])
	env.Append(LIBS = [ 
		"mingw32","m","dinput8","dxguid","dxerr8","user32",
		"gdi32","winmm","imm32","ole32","oleaut32","shell32",
		"version","uuid","opengl32","glu32", 
	])

	sconz.AddLibrary(env,
		["lib/SDL2-2.0.1/i686-w64-mingw32/include"],
		["lib/SDL2-2.0.1/i686-w64-mingw32/lib"],
		["SDL2","SDL2main"]
	)
	sconz.AddLibrary(env,
		["lib/SDL2_image-2.0.0/i686-w64-mingw32/include"],
		["lib/SDL2_image-2.0.0/i686-w64-mingw32/lib"],
		["SDL2_image"]
	)
	sconz.AddLibrary(env,
		["lib/glew-1.10.0/include"],
		["lib/glew-1.10.0/lib"],
		["glew32","glew32mx"]
	)

elif PLATFORM == "PLATFORM_OSX":
	pass

cpps = list(recursivefind.GetFiles(SRCDIR,"cpp"))

objects = env.Object(target = cpps)
main = env.Program(target = os.path.join(BINDIR,TARGET),source = objects)
run = env.AlwaysBuild(env.Alias("Run",[],'cd bin && start %s.exe'%TARGET))

Default(main)