.SUFFIXES:.c.o

CC=gcc
SRCS1=pthread_server.c
SRCS2=pthread_client.c
OBJS1=$(SRCS1:.c=.o)
OBJS2=$(SRCS2:.c=.o)
EXEC1=pthread_server_exec
EXEC2=pthread_client_exec
Idir=pthread

all:$(OBJS1) $(OBJS2)
	$(CC) -o $(EXEC1) $(OBJS1) -l $(Idir) 
	$(CC) -o $(EXEC2) $(OBJS2) -l $(Idir)
	@echo ----OK----
.c.o:
	$(CC) -o $@ -c $<
clean:
	rm -f core*
	rm -f $(OBJS1)	
	rm -f $(OBJS2)	
