OPT:=

CXX = u++						# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT} -DTYPE="${TYPE}" # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = Driver.o Helpers.o Args.o Bank.o Bottlingplant.o ConfigFile.o Nameserver.o Parent.o Printer.o Student.o Truck.o Vendingmachine.o Watcard.o Watcardoffice.o
EXEC1 = soda 

OBJECTS = ${OBJECTS1}               	# all object files
DEPENDS = ${OBJECTS:.o=.d}				# substitute ".o" with ".d"
EXECS = ${EXEC1}                    	# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}						# build all executables

${EXEC1} : ${OBJECTS1}					# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}				# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}					# include *.d files containing program dependences

clean :							# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
