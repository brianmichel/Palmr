C = m68k-palmos-gcc
CFLAGS = -Wall -O2 -g -fno-builtin
CSFLAGS = -S $(CFLAGS)

PILRC = pilrc
TXT2BITM = txt2bitm
OBJRES = m68k-palmos-obj-res
BUILDPRC = build-prc

PROGNAME = palmr
OBJS = $(PROGNAME).o

PILRCSRC = $(PROGNAME).rcp
ICONBMP = $(PROGNAME).pbitm

ICONTEXT = "Palmr"
APPID = PLMR

all: $(PROGNAME).prc

.S.o:
	$(CC) $(TARGETFLAGS) -c $<

.c.s:
	$(CC) $(CSFLAGS) $<

$(PROGNAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -lc -o $(PROGNAME)

$(PROGNAME).prc: $(PROGNAME) $(PILRCSRC) $(ICONBMP) Makefile
	$(OBJRES) $(PROGNAME)
	$(PILRC) $(PILRCSRC)
	$(TXT2BITM) $(ICONBMP)
	cp bin/*.bin .
	$(BUILDPRC) $(PROGNAME).prc $(ICONTEXT) $(APPID) code0001.$(PROGNAME).grc code0000.$(PROGNAME).grc data0000.$(PROGNAME).grc *.bin pref0000.$(PROGNAME).grc
	$(RM) -f *.bin *.grc

clean:
	$(RM) -rf $(PROGNAME).prc $(PROGNAME) *.o *.a *.bin *.grc
