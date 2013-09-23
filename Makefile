# Package information
LIBRARY = ExampleAnalysis
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include

# configure fastjet include:
INCLUDES += -I$(FASTJETDIR)/../include

#INCLUDES += -I$(LHAPDFDIR)/include
INCLUDES += -I/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.8/x86_64-slc5-gcc46-opt/include

# in case you need to link to an external library, use USERLDFLAGS, e.g. like that:
#USERLDFLAGS += $(ROOTLIBS) -lMinuit

# to pass additional compiler flags, set USERCXXFLAGS. Add debugging info here:
USERCXXFLAGS := -g -std=c++0x

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common

