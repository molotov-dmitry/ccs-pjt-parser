TARGET          := ccs-pjt-parser
INSTALLDIR      := $(PREFIX)/usr/local/bin

OBJDIR          := build/obj

CC              := g++

VPATH           := . export
INCLUDEPATH     := . export
DEFINES         := 

IFLAGS          := $(patsubst %,-I%,$(INCLUDEPATH))
DFLAGS          := $(patsubst %,-D%,$(DEFINES))

SOURCES         := $(wildcard *.cpp export/*.cpp)
OBJECTS         := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

CXXFLAGS        := -m32 -Wall -Wextra -pedantic -fPIC -O2 -Os -std=c++11 $(IFLAGS) $(DFLAGS)
LDFLAGS         := -m32 -lpthread

### ============================================================================

.PHONY: all clean install uninstall

all: $(TARGET)

### Build ======================================================================

$(TARGET): $(OBJDIR) $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
	strip $(TARGET)
	
clean:
	rm -rf $(OBJDIR)
	rm -f  $(TARGET)
	
### Deployment =================================================================
	
install: $(TARGET)
	install $(TARGET) $(INSTALLDIR)/$(TARGET)

uninstall:
	rm -f $(INSTALLDIR)/$(TARGET)
	
### Objects ====================================================================

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : %.cpp Makefile
	$(CC) $(CXXFLAGS) -c $< -o $@
