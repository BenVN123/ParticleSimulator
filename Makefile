CC = clang
CFLAGS = -g -Wall -Wextra -Wpedantic -I /opt/homebrew/include -fsanitize=address 
LDFLAGS = -L /opt/homebrew/lib -lSDL2

heheballs: main.o platform.o particles.o segments.o utils.o  
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.c main.h platform.h particles.h segments.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

platform.o: platform.c platform.h particles.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

particles.o: particles.c particles.h utils.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

segments.o: segments.c segments.h particles.h

utils.o: utils.c utils.h particles.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o heheballs 

.PHONY: clean
