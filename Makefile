# names of .cc files that have a main() function
#TARGETS = clockchain
TARGETS = main server client timer_test

# names of other .cc files
CXXFILES = Block Blockchain sha256

# Name of all boost libraries to link
BOOST_NAMES = system thread date_time regex serialization
BOOST_LIBS = $(addprefix boost_, $(BOOST_NAMES))
# Name of all libraries to link
LIBS = m crypto ssl pthread $(BOOST_LIBS)

# Let the programmer choose 32 or 64 bits, but default to 64 bit
BITS ?= 64

# Output folder
ODIR := ./obj$(BITS)
output_folder := $(shell mkdir -p $(ODIR))

# Names of the .o, .d, and .exe files
COMMONOFILES = $(patsubst %, $(ODIR)/%.o, $(CXXFILES))
ALLOFILES    = $(patsubst %, $(ODIR)/%.o, $(CXXFILES) $(TARGETS))
EXEFILES     = $(patsubst %, $(ODIR)/%.exe, $(TARGETS))
DFILES       = $(patsubst %.o, %.d, $(ALLOFILES))

# Basic tool configuration for GCC/G++.
#
# Note: These lines will require some changes in order to work with TBB
CXX      = g++
LD       = g++
CXXFLAGS = -MMD -O3 -m$(BITS) -ggdb -std=c++17 -Wall -Werror
LIBFLAGS = $(addprefix -l, $(LIBS))
LDFLAGS = -m$(BITS) $(LIBFLAGS)

# Standard build targets and rules follow
.DEFAULT_GOAL = all
.PRECIOUS: $(ALLOFILES)
.PHONY: all clean

all: $(EXEFILES)

$(ODIR)/%.o: %.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) $< -o $@ -c $(CXXFLAGS)

$(ODIR)/%.exe: $(ODIR)/%.o $(COMMONOFILES)
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	@echo Cleaning up...
	@rm -rf $(ODIR)

test: $(EXEFILES)
	@echo Running main.exe...
	@$(ODIR)/main.exe

-include $(DFILES)
