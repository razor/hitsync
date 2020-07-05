CC=gcc
CFLAGS=-pthread

hitsync: hitsync.c
	$(CC) $(CFLAGS) -o hitsync hitsync.c
	strip hitsync
	chmod +x hitsync
all: hitsync
clean:
	rm -rf hitsync
