#This MakeFile is based off of the one supplied by Z-Wave.me
#With modifications based on a solution posted by Yanick Rochon on stack overflow

### Variables ###
TARGET-C = SPFFT-client
TARGET-S = SPFFT-server

SRC_DIR = src
SAMPDIR = samples
OBJ_DIR = obj
BUILDDIR = build

SOURCES  := $(wildcard $(SRC_DIR)/*.c)
INCLUDES := $(wildcard $(SRC_DIR)/*.h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#LDFLAGS += -static
LIBS += -lm -lnanomsg
ARGS = -std=c99 -Wall -O3 

### Targets ###

all: $(OBJECTS)

run-client:
	(./$(BUILDDIR)/$(TARGET-C))

run-server:
	(./$(BUILDDIR)/$(TARGET-S))

run-valgrind-client:
	(valgrind --leak-check=full ./$(BUILDDIR)/$(TARGET-C))

run-gdb-client:
	(gdb ./$(BUILDDIR)/$(TARGET-C))

$(BUILDDIR)/$(TARGET-C): $(OBJECTS)
	$(CC) $(ARGS) $(SAMPDIR)/sampleClient.c $(LIBWSDIR) $(TARGET_LIBDIR) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< -I$(OBJECTS) $(LIBS)

$(BUILDDIR)/$(TARGET-S): $(OBJECTS)
	$(CC) $(ARGS) $(SAMPDIR)/sampleServer.c $(LIBWSDIR) $(TARGET_LIBDIR) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< -I$(OBJECTS) $(LIBS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(ARGS) $(TARGET_INCLUDES) $(TARGET_ARCH) -c $< -o $@

clean:
	rm $(BUILDDIR)/* $(OBJ_DIR)/*

.PHONY: all
