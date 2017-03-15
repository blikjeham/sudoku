SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

DEBUG = -ggdb
CFLAGS = -Wall -O2 $(DEBUG)
LDFLAGS = -lcurses

COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(CC) $(CFLAGS)
INSTALL = /usr/bin/install
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

DEPS = config.h s_util.h const.h ui.h
OBJ = main.o s_util.o bf_util.o ui.o

TEST_OBJ = test.o s_util.o bf_util.o ui.o

%.o: %.c $(DEPS)
	$(COMPILE) -c -o $@ $<

all: sudoku

sudoku: $(OBJ)
	 $(LINK) -o $@ $^ $(LDFLAGS)

# make test is just for debugging purposes
# since test.c usually only contains some
# test stuff
test: $(TEST_OBJ)
	$(LINK) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	\rm -f *.o *~ sudoku test
