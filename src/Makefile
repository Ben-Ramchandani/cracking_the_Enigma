CC=gcc
CFLAGS= -std=c99 -Wall -pedantic -O3
SOURCES=main.c Enigma.c enigma_helper.c plug_break.c
DEP=Enigma.h
OBJDIR=obj
EXEDIR=../bin
OBJECTS=$(patsubst %,$(OBJDIR)/%,$(SOURCES:.c=.o))
EXE=enigma

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE)

$(OBJDIR)/%.o: %.c $(DEP)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXE)

clean:
	-rm -f $(OBJDIR)/*.o $(EXE)
