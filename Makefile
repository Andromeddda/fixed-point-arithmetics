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

# Ask the compiler to write dependency information in .d file
DEPFLAGS = 	\
	-MMD 	\
	-MP 	\
	-MF $(DEPDIR)/$*.d

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

INCLUDES = \
	includes/long-number.hpp \
	includes/pi.hpp \
	includes/test_system.hpp \
	includes/tests.hpp \
	includes/utils.hpp

# Add "include" folder to header search path:
CFLAGS += -I $(abspath includes)

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
$(BUILD)/%.o: src/%.cpp $(INCLUDES) Makefile
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -c -o $@

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