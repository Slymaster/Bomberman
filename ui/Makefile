CC=gcc
CFLAGS=-lSDL2_image `sdl2-config --cflags --libs`
EXEC=game

all :
	$(CC) *.c $(CFLAGS) -o $(EXEC)
	./$(EXEC)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

