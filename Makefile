# Files to compile that don't have a main() function
CFILES = team support list

# Files to compile that do have a main() function
TARGETS = part1a part1b inout

# Files to compile to .so
CSOFILES = libpart1 libpart2 libpart3 libexploit libpart4

# Let the programmer choose 32 or 64 bits
BITS ?= 64

# Directory names
ODIR := ./obj$(BITS)
output_folder := $(shell mkdir -p $(ODIR))

# Names of files that the compiler generates
EXEFILES  = $(patsubst %, $(ODIR)/%,    $(TARGETS))
OFILES    = $(patsubst %, $(ODIR)/%.o,  $(CFILES))
SOFILES   = $(patsubst %, $(ODIR)/%.so, $(CSOFILES))
EXEOFILES = $(patsubst %, $(ODIR)/%.o,  $(TARGETS))
DEPS      = $(patsubst %, $(ODIR)/%.d,  $(CFILES) $(CSOFILES) $(TARGETS))

# Use gcc and g++
CC = gcc
CFLAGS = -MMD -O2 -m$(BITS) -ggdb -D_GNU_SOURCE -std=gnu11
SOFLAGS = -fPIC -shared
LDFLAGS = -m$(BITS) -ldl

# Best to be safe...
.DEFAULT_GOAL = all
.PRECIOUS: $(OFILES) $(EXEOFILES)
.PHONY: all clean

# Goal is to build all executables and shared objects
all: $(EXEFILES) $(SOFILES)

# Rules for building object files
$(ODIR)/%.o: %.c
	@echo "[CC] $< --> $@"
	@$(CC) $< -o $@ -c $(CFLAGS)

# Rules for building executables
$(ODIR)/%: $(ODIR)/%.o $(OFILES)
	@echo "[LD] $< --> $@"
	@$(CC) $^ -o $@ $(LDFLAGS)

# Special rule for part1a, since it needs libpart1 on its library path
$(ODIR)/part1a: $(ODIR)/part1a.o $(OFILES) $(SOFILES)
	@echo "[LD] $< --> $@"
	@$(CC) $^ -o $@ $(LDFLAGS) -L$(ODIR)/ -lpart1

# Rules for building shared objects
$(ODIR)/%.so: %.c
	@echo "[CC] $< --> $@"
	@$(CC) $< -o $@ $(CFLAGS) $(SOFLAGS)
# NB: custom rule for libpart2.so
$(ODIR)/libpart2.so: libpart2.c $(ODIR)/list.o
	@echo "[CC] $^ --> $@"
	@$(CC) $^ -o $@ $(CFLAGS) $(SOFLAGS)

# clean by clobbering the build folder
clean:
	@echo Cleaning up...
	@rm -rf $(ODIR)

# turn in via script
turnin:
	@echo "Submitting..."
	/proj/spear0/cse303-submit-p3.pl

# Include dependencies
-include $(DEPS)
