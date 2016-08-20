XCC = m68k-palmos-gcc
XCFLAGS = -O2 -Wall -g -fno-builtin
XCSFLAGS = -S $(CFLAGS)

PILRC = pilrc
PILRCOPT = -q -ro

TXT2BITM = txt2bitm
OBJRES = m68k-palmos-obj-res
BUILDPRC = build-prc

PROGNAME = db
OBJS = $(PROGNAME).o

PILRCSRC = $(PROGNAME).rcp
ICONBMP = $(PROGNAME).pbitm

ICONTEXT = "db"
APPID = DB

all: $(PROGNAME).prc

$(PROGNAME).prc: $(PROGNAME) bin.stamp
	build-prc $(PROGNAME).prc "db practice" TuHe $(PROGNAME) *.bin

$(PROGNAME): $(PROGNAME).o
	$(XCC) $(XCFLAGS) -o $(PROGNAME) $(PROGNAME).o

$(PROGNAME).o: $(PROGNAME).c $(PROGNAME).h
	$(XCC) $(XCFLAGS) -c $(PROGNAME).c

bin.stamp: $(PROGNAME).rcp $(PROGNAME).h $(PROGNAME).bmp
	pilrc $(PROGNAME).rcp

clean:
	-rm -f *.[oa] $(PROGNAME) *.bin *.stamp
