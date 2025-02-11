CC 			  = gcc
ARR			  = ar

VESRION_C     = 11

PATH_SRC 	      = src
PATH_INCLUDE      = include
PATH_EXAMPLES	  = example
PATH_LIBRARY      = lib

PATH_DEBUG 		  	= $(PATH_LIBRARY)/DebugLibC
PATH_COLORS		  	= $(PATH_DEBUG)/colors-C-C-plus-plus

PATH_INSTRUCTION  = instructions

LINKER_FLAGS  	    = 									\
	-L. 						-lbigint	 			\
	-L$(PATH_DEBUG)  			-ldebug 				\
	-L$(PATH_COLORS) 			-lcolors

INCLUDE_FLAGS = 										\
	-I.													\
	-I$(PATH_INCLUDE)									\
	-I$(PATH_DEBUG)										\
	-I$(PATH_COLORS)

GLOBAL_CFLAGS = -std=c$(VESRION_C) $(INCLUDE_FLAGS) -masm=intel \
				-D_ExceptionHandler -fdiagnostics-color=always  \
				 -D_GNU_SOURCE $(DEBUG_LINUX)

CFLAGS 		  =  $(GLOBAL_CFLAGS) -O3 -Wno-unused-parameter \
				-Wno-implicit-fallthrough -Wno-type-limits  \
				-Wno-unused-variable -Wno-pointer-sign

CFLAGS_DEBUG  =  $(GLOBAL_CFLAGS) -ggdb -fno-asynchronous-unwind-tables  	      	\
				-Wall -Wextra -pipe -O0 -D DEBUG_ENABLE -fno-stack-protector  		\
				-fstack-protector-strong -Wpedantic -fno-omit-frame-pointer       	\
				-fno-inline -fno-optimize-sibling-calls -fdiagnostics-show-option -fPIC

ARR_FLAGS     = -rc

OBJECTS = 	BigInt.o Fractions.o
OBJECTS_DEBUG = BigInt_debug.o Fractions_debug.o