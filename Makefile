#This MakeFile is based off of the one supplied by Z-Wave.me
#With modifications based on a solution posted by Yanick Rochon on stack overflow

### Variables ###
TARGET = SPFFT-client

SRC_DIR = src
SAMPDIR = samples
OBJ_DIR = obj
BUILDDIR = build

SOURCES  := $(wildcard $(SRC_DIR)/*.c)
SOURCES  += $(SAMPDIR)/sampleClient.c
INCLUDES := $(wildcard $(SRC_DIR)/*.h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#LDFLAGS += -static
LIBS += -lm -lSDL2 -lSDL2_net
ARGS = -std=c99 -Wall -O3 

### Targets ###

all: $(TARGET)

run:
	(./$(BUILDDIR)/$(TARGET))

run-valgrind:
	(valgrind --leak-check=full ./$(BUILDDIR)/$(TARGET))

run-gdb:
	(gdb ./$(BUILDDIR)/$(TARGET))

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(ARGS) $(LIBWSDIR) $(TARGET_LIBDIR) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< -I$(OBJECTS) $(LIBS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(ARGS) $(TARGET_INCLUDES) $(TARGET_ARCH) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean run full_clean
