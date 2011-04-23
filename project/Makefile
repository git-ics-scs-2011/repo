TARGET =	emulator
SOURCES = 	main.c \
		cpu_emu/cpu_emu.c \
		prj_core/prj_core.c
INCLUDE = -I.
LDFLAGS = -lpthread
CFLAGS = -g -O0 -std=c99
CC = gcc


all: $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES) $(LDFLAGS) $(CFLAGS) $(INCLUDE)
clean:
	rm -rf $(TARGET)
