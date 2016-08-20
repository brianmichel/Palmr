CC = m68k-palmos-gcc
CFLAGS = -O2 -palmos3.5

all: db.prc

db.prc: db bin.stamp
	build-prc db.prc "Database" TuDb db *.bin

db: db.o
	$(CC) $(CFLAGS) -o db db.o

db.o: db.c db.h
	$(CC) $(CFLAGS) -c db.c

bin.stamp: db.rcp db.h db.bmp
	pilrc db.rcp

clean:
	-rm -f *.[oa] db *.bin *.stamp
