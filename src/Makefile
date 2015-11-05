CC=gcc
CFLAGS= -std=c99 -Wall -pedantic -O3
SOURCES=Enigma.c enigma_helper.c plug_break.c
DEP=Enigma.h
OBJDIR=obj
EXEDIR=../bin
OBJECTS=$(patsubst %,$(OBJDIR)/%,$(SOURCES:.c=.o))
EXE=enigma

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE)

$(OBJDIR)/%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	-rm -f $(OBJDIR)/*.o $(EXE)
