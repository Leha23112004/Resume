CFLAGS=-Wall -Wextra -Werror

TARGETS := lab12 lab12lib

all: $(TARGETS)

clean: 
	rm -rf *.o lab12araN3250 libaraN3250.so

lab12: main.c plugin_api.h
	gcc $(CFLAGS) -o lab12araN3250 main.c -ldl -g

lab12lib: libaraN3250.c plugin_api.h
	gcc $(CFLAGS) -shared -fPIC -o libaraN3250.so libaraN3250.c -ldl -lm -g
