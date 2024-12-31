CC = gcc
CFLAGS = -static -Wall -Wextra -std=c11 -g
SRCS = shell.c core.c
DEFS = core.h
EXEC = shell
OUTDIR = out

.PHONY: all clean run

all: $(OUTDIR)/$(EXEC)

$(OUTDIR)/$(EXEC): $(SRCS) $(DEFS)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUTDIR)/$(EXEC)

run: $(OUTDIR)/$(EXEC)
	$(OUTDIR)/$(EXEC)

clean:
	rm -f $(OUTDIR)/$(EXEC)
