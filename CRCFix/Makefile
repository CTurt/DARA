CC		:=	gcc
SDIR	:=	source
IDIR	:=	include
CFLAGS	:=	-I$(IDIR) -fms-extensions
ODIR	:=	build
CFILES	:=	$(wildcard $(SDIR)/*.c)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(wildcard $(SDIR)/*.c))

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	OUT=CRCFix
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	OUT=CRCFix
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	OUT=CRCFix.exe
endif

$(OUT): $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o $(OUT) $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(OUT) $(ODIR)/*.o
