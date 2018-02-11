
all: Initiator

OBJS1	= Client.o GlobalInfo.o main.o ProcessInput.o RequestList.o
OUT1	= Initiator

CC	= gcc
FLAGS	= -g -c -Wall -pthread

# OBJECTS

Client.o: Client.c Client.h
	$(CC) $(FLAGS)  Client.c Client.h

GlobalInfo.o: GlobalInfo.h GlobalInfo.c
	$(CC) $(FLAGS)  GlobalInfo.h GlobalInfo.c

main.o: main.c
	$(CC) $(FLAGS)  main.c

ProcessInput.o: ProcessInput.h ProcessInput.c
	$(CC) $(FLAGS)  ProcessInput.h ProcessInput.c

RequestList.o: RequestList.h RequestList.c
	$(CC) $(FLAGS)  RequestList.h RequestList.c




Initiator: $(OBJS1)
	$(CC) -pthread -g -Wall  $(OBJS1) -o $(OUT1)



# clean up
clean:
	rm -f $(OBJS1) $(OUT1)
	rm -rf *.gch
