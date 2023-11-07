CC = gcc
LDFLAGS = -lm
SRC = main.c reader.c display.c
TARGET = output

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean