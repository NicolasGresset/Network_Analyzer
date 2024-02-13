TARGET = sniffer


SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin
INCLUDE_PATH := include

CFLAGS := -Wall -Wextra -g -pedantic
LDLIBS := -lpcap

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	gcc -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "Linking complete"

$(OBJECTS) : $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	mkdir -p $(OBJDIR)
	gcc -o $@ -c $< $(CFLAGS) $(LDLIBS)



dir:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(SRCDIR)
	mkdir -p $(INCLUDE_PATH)

clean:
	rm -rf $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(TARGET)