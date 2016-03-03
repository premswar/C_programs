
APP =sigs
# I am a comment, and I want to say that the variable CC will be
# # the compiler to use.
CC=gcc
# # Hey!, I am comment number 2. I want to say that CFLAGS will be the
# # options I'll pass to the compiler.
CFLAGS=-c 

all:    libsig_lib.a 
	$(CC) -o $(APP) mysig.c -lsig_lib -L. -lpthread 
libsig_lib.a: sig_lib.o
	ar -crv libsig_lib.a sig_lib.o
sig_lib.o: sig_lib.c
	$(CC) $(CFLAGS) sig_lib.c

clean:
	rm -f $(APP)
	rm -f libsig_lib.a
	rm -rf *.o

