CFLAGS=--std=c17 -Wall -pedantic -Isrc/ -ggdb -Wextra -Werror -DDEBUG
BUILDDIR=build
TESTDIR=test
SRCDIR=src
CC=gcc

all: build test

test: $(BUILDDIR)/test.o $(BUILDDIR)/mem.o $(BUILDDIR)/util.o
	$(CC) -o $(TESTDIR)/test $^
	./$(TESTDIR)/test


build:
	mkdir -p $(TESTDIR)
	mkdir -p $(BUILDDIR)


$(BUILDDIR)/mem.o: $(SRCDIR)/mem.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/util.o: $(SRCDIR)/util.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/test.o: $(SRCDIR)/test.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TESTDIR)

