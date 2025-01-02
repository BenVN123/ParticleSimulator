CC = clang
CFLAGS = -g -Wall -Wextra -Wpedantic -I /opt/homebrew/include -fsanitize=address 
LDFLAGS = -L /opt/homebrew/lib -lSDL2

heheballs: main.o platform.o 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.c main.h platform.c platform.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

platform.o: platform.c platform.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o heheballs 

.PHONY: clean
