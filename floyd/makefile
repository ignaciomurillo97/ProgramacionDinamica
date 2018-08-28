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
OPT=-O0 -O2
# warnings
WARN=-Wall

PTHREAD=-pthread

LIBS=-L/usr/lib -lcgraph -lgvc -lgraph 

CCFLAGS=$(LIBS) $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GVCLIB=`=‘pkg-config libgvc --cflag` `pkg-config libgvc --lib`
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
