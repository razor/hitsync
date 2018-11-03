CC=gcc

hitsync: hitsync.c
	$(CC) -o hitsync hitsync.c
	strip hitsync
	chmod +x hitsync
all: hitsync
clean:
	rm -rf hitsync
