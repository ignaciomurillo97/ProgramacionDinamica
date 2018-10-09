# change application name here (executable output name)
TARGET=reemplazo

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

LIBS=-L/usr/lib -lm

CCFLAGS=$(LIBS) $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0` 

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

all: $(OBJECTS)
	$(LD) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $(CCFLAGS) $< $(GTKLIB) -o $@

echo:
	echo $(SOURCES)
	ls $(OBJ)

clean:
	rm -f $(OBJ)/* $(TARGET)
