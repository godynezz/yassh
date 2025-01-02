CC = gcc
CFLAGS = -static -Wall -Wextra -std=c11 -g
SRCS = shell.c core.c
DEFS = core.h
EXEC = yassh
OUTDIR = out

.PHONY: all clean run

all: $(OUTDIR)/$(EXEC)

$(OUTDIR)/$(EXEC): $(SRCS) $(DEFS)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUTDIR)/$(EXEC)

run: $(OUTDIR)/$(EXEC)
	$(OUTDIR)/$(EXEC)

clean:
	rm -rf $(OUTDIR)

install: $(OUTDIR)/$(EXEC)
	sudo install -m 555 $(OUTDIR)/$(EXEC) /usr/local/bin/$(EXEC)