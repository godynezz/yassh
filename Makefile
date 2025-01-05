CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
SRCS = shell.c core.c
DEFS = core.h
EXEC = yassh
OUTDIR = out

.PHONY: all clean run install

all: $(OUTDIR)/$(EXEC)

$(OUTDIR)/$(EXEC): $(SRCS) $(DEFS)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUTDIR)/$(EXEC)

run: $(OUTDIR)/$(EXEC)
	$(OUTDIR)/$(EXEC)

clean:
	rm -rf $(OUTDIR)

install: $(OUTDIR)/$(EXEC)
	sudo rm -f -- /usr/local/bin/$(EXEC)
	sudo install -m 555 $(OUTDIR)/$(EXEC) /usr/local/bin/$(EXEC)