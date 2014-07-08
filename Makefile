ifdef CROSS_COMPILE
	SYSROOT=/home/christopher/rpi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/../arm-linux-gnueabihf/libc
	CC=arm-linux-gnueabihf-c++
	CFLAGS=-c -Wall -I$(SYSROOT)/usr/include -I$(SYSROOT)/usr/local/include
else
	CC=/usr/bin/c++
	CFLAGS=-c -Wall -fPIC
endif

ifndef INSTALL
	INSTALL=/usr/local
endif

LDFLAGS= -lmosquittopp -lPocoFoundation -shared
OBJ=obj
BIN=lib
SOURCES=MQTTClient.cpp
OBJECTS=$(addprefix $(OBJ)/,$(SOURCES:.cpp=.o))

LIBRARY=$(BIN)/libMQTTClient.so
HEADER=MQTTClient.h

.PHONY: all
.PHONY: clean
.PHONY: build

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS) | $(BIN)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS): $(OBJ)/%.o: %.cpp %.h | $(OBJ)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

$(OBJ):
	-mkdir $(OBJ)

$(BIN):
	-mkdir $(BIN)

install: $(LIBRARY)
	cp $(LIBRARY) $(INSTALL)/lib
	cp $(HEADER) $(INSTALL)/include