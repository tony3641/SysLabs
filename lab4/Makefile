TARGET = main
OBJS = bounded_buffer.o main.o
CFLAGS += -Wall -Wextra -std=c11 -lpthread

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)


.PHONY: debug
debug: list
debug: CFLAGS += -fprofile-arcs -ftest-coverage

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).out