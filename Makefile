# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Werror

# Source files
SRCDIR = lib
SOURCES = $(SRCDIR)/*.cpp main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = exame

# Libraries
LIBS = -lncurses

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
