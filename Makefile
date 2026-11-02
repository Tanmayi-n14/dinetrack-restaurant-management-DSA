CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = dinetrack
SRC = src/DineTrack.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
