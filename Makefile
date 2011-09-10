SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

DEBUG = -ggdb
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

INSTALL = /usr/bin/install
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

DEPS = s_util.h const.h
OBJ = main.o s_util.o

TEST_OBJ = test.o s_util.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

all: sudoku

sudoku: $(OBJ)
	 $(CC) $(LFLAGS) -o $@ $^

# make test is just for debugging purposes
# since test.c usually only contains some
# test stuff
test: $(TEST_OBJ)
	$(CC) $(LFLAGS) -o $@ $^

.PHONY: clean

clean:
	\rm -f *.o *~ sudoku test
