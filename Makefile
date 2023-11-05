#
# Created by gmakemake (Ubuntu Jul 25 2014) on Sat Oct 28 12:26:29 2023
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CFLAGS =	-std=c99 -ggdb -Wall -Wextra -pedantic
CLIBFLAGS =	-lm  -lcurses 


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	agent.c bracetopia.c simulation.c use_getopt.c
PS_FILES =	
S_FILES =	
H_FILES =	agent.h simulation.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	agent.o simulation.o 

#
# Main targets
#

all:	bracetopia use_getopt 

bracetopia:	bracetopia.o $(OBJFILES)
	$(CC) $(CFLAGS) -o bracetopia bracetopia.o $(OBJFILES) $(CLIBFLAGS)

use_getopt:	use_getopt.o $(OBJFILES)
	$(CC) $(CFLAGS) -o use_getopt use_getopt.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

agent.o:	agent.h
bracetopia.o:	agent.h simulation.h
simulation.o:	agent.h simulation.h
use_getopt.o:	

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) bracetopia.o use_getopt.o core

realclean:        clean
	-/bin/rm -f bracetopia use_getopt 
