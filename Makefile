cc=gcc
cflags = -c -fPIC -Wall
libsrc= dmi.c 
libobj= dmi.o 
libname= libdmi.a
libsoname= libdmi.so
all: lib dlib

lib: $(libsrc) 
	$(cc) $(cflags) $^
	ar -crs libdmi.a $(libobj) 

dlib: $(libsrc)
	$(cc) $(libobj) -shared -o $(libsoname) 

.PHONY: clean

clean:
	@rm $(libobj) $(libsoname) $(libname) a.out
