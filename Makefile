CFLAGS +=-Wall -Wextra
CFLAGS += -Og -g
CFLAGS += -std=gnu11

all: err-test
err-test: err-test.o err.o

clean:
	rm -f *.o err-test
