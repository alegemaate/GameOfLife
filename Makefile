CC = gcc
CFLAGS = -I.
LIBS = -lalleg44
DEPS = src/main.c
ODIR = obj
SDIR = src

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build/GameOfLife: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
  
.PHONY: clean
  
clean:
	rm -f $(ODIR)/*.o build/*.exe