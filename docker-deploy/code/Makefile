TARGET=proxy_server
CC=g++
CPPFLAGS=-std=c++11 -pedantic -Wall -Werror -ggdb3 -g -pthread
DEBUG_MODE=NONE

all: $(TARGET)
proxy_server:
	$(CC) $(CPPFLAGS) -D$(DEBUG_MODE) -o $@ proxy_server.cpp http_message.cpp http_request.cpp http_response.cpp cache.cpp
.PHONY: clean depend
clean:
	rm -f $(TARGET) *.o *.cpp~ *.h~
