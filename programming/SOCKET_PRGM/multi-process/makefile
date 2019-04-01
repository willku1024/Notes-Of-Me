.SUFFIXES:.c.o

CC=gcc
SRCS1=server.c
SRCS2=client.c
OBJS1=$(SRCS1:.c=.o)
OBJS2=$(SRCS2:.c=.o)
EXEC1=server_exec
EXEC2=client_exec

all:$(OBJS1) $(OBJS2)
	$(CC) -o $(EXEC1) $(OBJS1) 
	$(CC) -o $(EXEC2) $(OBJS2) 
	@echo ----OK----
.c.o:
	$(CC) -o $@ -c $<
clean:
	rm -f core*
	rm -f $(OBJS1)	
	rm -f $(OBJS2)	
