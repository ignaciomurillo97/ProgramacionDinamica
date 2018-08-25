# change application name here (executable output name)
TARGET=rutasOptimas

# folders
SRC=src
OBJ=obj
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

all: $(OBJECTS)
	$(LD) -o $(TARGET) $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -I$(SRC) -c $(CCFLAGS) $< $(GTKLIB) -o $@

echo:
	echo $(SOURCES)
	ls $(OBJ)

clean:
	rm -f $(OBJ)/* $(TARGET)
