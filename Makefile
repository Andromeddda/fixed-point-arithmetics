#----------
# Compiler
#----------

CC = g++

#-------------
# Directories
#-------------

BUILD = build
DEPDIR := $(BUILD)/deps
INCDIR = includes

#-------
# Flags
#-------

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-std=c++17 \
 	-O3

# Ask the compiler to write dependency information in .d file
CFLAGS += -I $(INCDIR)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

#--------
# Colors
#--------

# Use ANSI color codes:
BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Files
#-------

# List of sources:
TEST = test
PI = pi

# Sources function 
# NOTE: we do not separate sources with main() and without here,
# so that DEPFILES will contain all .d files
SOURCES = $(notdir $(wildcard src/*.cpp))

# Object files:

TEST_OBJ = $(BUILD)/$(TEST).o
PI_OBJ = $(BUILD)/$(PI).o

# Separate objects with and withot main()
OBJECTS := $(filter-out $(TEST_OBJ) $(PI_OBJ), $(SOURCES:%.cpp=$(BUILD)/%.o))

# Executable files:
TEST_EXECUTABLE = $(BUILD)/$(TEST)
PI_EXECUTABLE = $(BUILD)/$(PI)


#---------------
# Build process
#---------------

# By default, build executables:
default: $(TEST_EXECUTABLE) $(PI_EXECUTABLE) 

# Link all object files together to obtain a binary test:
$(TEST_EXECUTABLE): $(OBJECTS) $(TEST_OBJ) 
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@

# Link all object files together to obtain a binary test:
$(PI_EXECUTABLE): $(OBJECTS) $(PI_OBJ)
	@printf "$(BYELLOW)Linking executable pi $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@

# Compile an object file:
$(BUILD)/%.o: src/%.cpp
$(BUILD)/%.o: src/%.cpp $(DEPDIR)/%.d Makefile | $(DEPDIR)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	$(CC) $(DEPFLAGS) $(CFLAGS) $< -c -o $@
	$(POSTCOMPILE)

# Create directory
$(DEPDIR) : ; @mkdir -p $(DEPDIR)

# Dependency files
DEPFILES := $(SOURCES:%.cpp=$(DEPDIR)/%.d)

# Mention every .d file as a target so that Make won't fail if it doesn't exist
$(DEPFILES):

# Include all created dependencies in current makefile
include $(wildcard $(DEPFILES))

#-----------------
# Run the program
#-----------------

# Run test:
test: $(TEST_EXECUTABLE)
	@mkdir -p res
	./$(TEST_EXECUTABLE)

# Run pi
pi: $(PI_EXECUTABLE)
	@mkdir -p res
	./$(PI_EXECUTABLE)

#-------
# Other
#-------

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf $(BUILD)

# List of non-file targets:
.PHONY: test pi clean default
