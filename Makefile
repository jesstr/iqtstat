TARGET = demo

.PHONY: all clean

SRC = main.c
SRC += fpid.c
SRC += tres.c

CC = gcc
CFLAGS = -I.

BUILDDIR = ./build
OUTPUTDIR = ./output

COBJ = $(addprefix $(BUILDDIR)/, $(SRC:.c=.o))
BIN = $(addprefix $(BUILDDIR)/, $(TARGET))

all: builddir $(BIN)
	@mkdir -p $(OUTPUTDIR)
	@cp $(BIN) $(OUTPUTDIR)

builddir:
	@mkdir -p $(BUILDDIR)

$(COBJ): $(BUILDDIR)/%.o : %.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(COBJ)
	@echo Linking $@
	@$(CC) -o $(BUILDDIR)/$(TARGET) $(COBJ)

clean:
	rm -rf $(BUILDDIR) $(OUTPUTDIR)