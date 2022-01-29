
CC=gcc
CFLAGS=--std=c17 -Wall -pedantic -Isrc/ -ggdb -Wextra -Werror -DDEBUG

BUILDDIR=build
TESTDIR=test
SRCDIR=src

.PHONY: all clean test

all: $(BUILDDIR)/mem.o $(BUILDDIR)/util.o $(BUILDDIR)/mem_debug.o $(BUILDDIR)/main.o
	$(CC) -o $(BUILDDIR)/main $^

build:
	mkdir -p $(TESTDIR)
	mkdir -p $(BUILDDIR)


$(BUILDDIR)/mem.o: $(SRCDIR)/mem.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/mem_debug.o: $(SRCDIR)/mem_debug.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/util.o: $(SRCDIR)/util.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/test.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/test.o: $(SRCDIR)/test.c build
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TESTDIR)

test: $(BUILDDIR)/main.o $(BUILDDIR)/mem.o $(BUILDDIR)/util.o $(BUILDDIR)/test.o
	@+cd tester; make CC=$(CC)
	$(CC) -o $(TESTDIR)/main $^
	./$(TESTDIR)/main
