
srcdir = ./src
prefix = ./build

CFLAGS = -Wall

$(prefix):
	mkdir $(prefix);

server: $(prefix)
	$(CC) $(CFLAGS) $(srcdir)/server.c -o $(prefix)/server.exe

client: $(prefix)
	$(CC) $(CFLAGS) $(srcdir)/client.c -o $(prefix)/client.exe

clean:
	rm -rf ./build

.PHONY: clean