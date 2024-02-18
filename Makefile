#----------
# Compiler
#----------

CC = g++

#-------------
# Directories
#-------------

BUILD = build
DEPDIR = $(BUILD)/deps
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

CFLAGS += -I $(INCDIR)

# Ask the compiler to write dependency information in .d file
DEPFLAGS = \
	-MM \
	-MG \
	-MP \
	-MF $(DEPDIR)/$*.d

# -MM for tracking "#include"-s
# -MG for avoiding errors if file not found
# -MP for mentioning all prerequisites as targets
# -MF for writing dependencies in .d file

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
TEST_SRC = test.cpp
PI_SRC = pi.cpp

# Sources without main() function
SOURCES = $(filter-out $(TEST_SRC) $(PI_SRC), $(notdir $(wildcard src/*.cpp)))

# Object files:
OBJECTS = $(SOURCES:%.cpp=$(BUILD)/%.o)

TEST_OBJ = $(TEST_SRC:%.cpp=$(BUILD)/%.o)
PI_OBJ = $(PI_SRC:%.cpp=$(BUILD)/%.o)

# Executable files:
TEST_EXECUTABLE = $(BUILD)/test
PI_EXECUTABLE = $(BUILD)/pi

# Dependency files
DEPFILES = $(SOURCES:%.cpp=$(DEPDIR)/%.d)

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
$(BUILD)/%.o: src/%.cpp Makefile | $(DEPDIR)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	@$(CC) $(DEPFLAGS) $< -c
	$(CC) $(CFLAGS) $< -c -o $@

# Create directory
$(DEPDIR) : ; @mkdir -p $(DEPDIR)

# Mention every .d file as a target so that make won't fail if it doesn't exist
$(DEPFILES) : ;

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
