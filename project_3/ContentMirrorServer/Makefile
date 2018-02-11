
all: Server

OBJS2	= Client.o GlobalInfo.o main.o Request.o Server.o RequestList.o MirrorManager.o ContentProvider.o MirrorWorker.o Utilities.o ProcessInput.o ListOfThreads.o
OUT2	= Server

CC	= gcc
FLAGS	= -g -c -Wall -pthread



# OBJECTS

Client.o: Client.c Client.h
	$(CC) $(FLAGS) Client.c Client.h

GlobalInfo.o: GlobalInfo.h GlobalInfo.c
	$(CC) $(FLAGS) GlobalInfo.h GlobalInfo.c

main.o: main.c
	$(CC) $(FLAGS) main.c

Request.o: Request.h Request.c
	$(CC) $(FLAGS) Request.h Request.c

Server.o: Server.h Server.c
	$(CC) $(FLAGS) Server.h Server.c

RequestList.o: RequestList.h RequestList.c
	$(CC) $(FLAGS) RequestList.h RequestList.c

MirrorManager.o: MirrorManager.h MirrorManager.c
	$(CC) $(FLAGS) MirrorManager.h MirrorManager.c

ContentProvider.o: ContentProvider.h ContentProvider.c
	$(CC) $(FLAGS) ContentProvider.h ContentProvider.c

MirrorWorker.o: MirrorWorker.h MirrorWorker.c
	$(CC) $(FLAGS) MirrorWorker.h MirrorWorker.c

Utilities.o: Utilities.h Utilities.c
	$(CC) $(FLAGS) Utilities.h Utilities.c

ProcessInput.o: ProcessInput.h ProcessInput.c
	$(CC) $(FLAGS) ProcessInput.h ProcessInput.c

ListOfThreads.o: ListOfThreads.h ListOfThreads.c
	$(CC) $(FLAGS) ListOfThreads.h ListOfThreads.c



Server: $(OBJS2)
	$(CC) -pthread -g -Wall  $(OBJS2) -o $(OUT2)



# clean up
clean:

	rm -f $(OBJS2) $(OUT2)
	rm -rf *.gch
