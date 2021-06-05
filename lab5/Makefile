CC = gcc
OBJS = main.o bounded_buffer.o hashmap.o word_count.o
LINKOBJ = main.o bounded_buffer.o hashmap.o word_count.o
TARGET = lab5
CFLAGS = -std=gnu99 -lpthread -ggdb3

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(LINKOBJ) $(TARGET) *.gcda *.gcno