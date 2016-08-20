CC = m68k-palmos-gcc
CFLAGS = -O2 -palmos3.5

all: forms.prc

forms.prc: forms bin.stamp
	build-prc forms.prc "Forms" TuFo forms *.bin

forms: forms.o
	$(CC) $(CFLAGS) -o forms forms.o

forms.o: forms.c forms.h
	$(CC) $(CFLAGS) -c forms.c

bin.stamp: forms.rcp forms.h forms.bmp
	pilrc forms.rcp

clean:
	-rm -f *.[oa] forms *.bin *.stamp
