CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lcv -lcxcore -lhighgui
CVDIR=./cvblob/
SOURCES=main.cpp spritecutter.cpp \
$(CVDIR)cvblob.cpp \
$(CVDIR)cvcontour.cpp $(CVDIR)cvlabel.cpp \
$(CVDIR)cvtrack.cpp $(CVDIR)cvaux.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=spritecutter

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o spritecutter
