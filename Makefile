CC = g++

CFLAGS = 					\
	-Wall 					\
	-Werror 				\
	-Wextra 				\
	-std=c++17				\


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
	includes/long-number.hpp 	\
	includes/utils.hpp			\
	includes/test_system.hpp	\
	includes/tests.hpp			

# Add "include" folder to header search path:
CFLAGS += -I $(abspath includes)

# List of sources:
SOURCES = \
	main.cpp 		\
	long-number.cpp	\
	test_system.cpp \
	tests.cpp

OBJECTS = $(SOURCES:%.cpp=build/%.o)

EXECUTABLE = build/main


#---------------
# Build process
#---------------

# By default, build executable:
default: $(EXECUTABLE) Makefile

# Link all object files together to obtain a binary:
$(EXECUTABLE): $(OBJECTS) Makefile
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Compile an object file:
build/%.o: src/%.cpp $(INCLUDES) Makefile
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CC) -c $< $(CFLAGS) -o $@

#--------------
# Test scripts
#--------------

# Run program:
run: $(EXECUTABLE) Makefile
	@mkdir -p res
	./$(EXECUTABLE)

#-------
# Other
#-------

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf build

# List of non-file targets:
.PHONY: run clean default