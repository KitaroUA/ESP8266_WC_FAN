TARGET = mkupgimg








#CC = gcc
#LD = $(CC)
#CFLAGS=-c -std=gnu99
CFLAGS=-c -std=c99


mkupgimg: mkupgimg.c	$(CC) -o $@ $^

httpd: mkupgimg.c 	$(CC) -o $@ $^
all: mkupgimg.c   	$(CC) -o $@ $^
flash: mkupgimg.c 	$(CC) -o $@ $^

clean:
	rm -f mkupgimg
