CC = g++
#LDLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU -lGLEW -lXmu -lX11 -lXi -lm
LDLIBS = -lGL -lglut -lGLEW

BIN_DIR  = ../../bin
BUILD_DIR  = ../../build
SRC_DIR  = ../../src

EXECUTABLE_FILE = flowerofpersia

TYPES_H = common/types.h
MATH_H = common/math.h

all: $(BIN_DIR)/$(EXECUTABLE_FILE)

$(BIN_DIR)/$(EXECUTABLE_FILE): $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/main.o $(LDLIBS) -o $@

$(BUILD_DIR)/main.o:
	$(CC) $(SRC_DIR)/main.cpp $(SRC_DIR)/main.h -I../../../common -o $@

clean:
	rm -rf $(BUILD_DIR)/main.o $(BIN_DIR)/$(EXECUTABLE_FILE)
