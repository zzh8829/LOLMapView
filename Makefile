# Makefile
# Zihao Zhang
# :D

CC			= gcc
CXX			= g++

INC_DIR		= include
SRC_DIR		= source
BIN_DIR		= bin
BUILD_DIR	= build

ifeq ($(OS),Windows_NT)
	include Makefile.win
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		include Makefile.osx
	endif
endif

SRCS = $(wildcard ${SRC_DIR}/*.cpp)
OBJS = $(subst ${SRC_DIR}/,${BUILD_DIR}/,${SRCS:.cpp=.o})

INCFLAG 	= $(addprefix -I,${INC_DIR_ALL})
LIBFLAG		= $(addprefix -L,${LIB_DIR_ALL}) $(addprefix -l,${LIB_ALL}) \
				${LIB_EXTRA}
STDFLAG 	= -std=gnu++11
OPTFLAG 	= -O3
WARNING 	= -Wall -Wextra
NOWARNING 	= -Wno-unused-function -Wno-unused-parameter -Wno-sign-compare \
				-Wno-trigraphs -Wno-unused-variable \
				-Wno-unused-but-set-variable

CXXFLAGS	= ${OPTFLAG} ${INCFLAG} ${STDFLAG} \
				${WARNING} ${NOWARNING}

EXE 		= ${BIN_DIR}/main

.PHONY: clean run

all : ${EXE} run

${EXE} : ${OBJS}
	${CXX} ${OBJS} ${LIBFLAG} -o $@

${BUILD_DIR}/%.o : ${SRC_DIR}/%.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $< 

.DEFAULT_GOAL := all