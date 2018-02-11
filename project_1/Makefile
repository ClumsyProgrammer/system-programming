
OBJS1	= main.o CallRecord.o RecordList.o HashTable.o UserInput.o HashFunction.o RecordManagement.o Global.o MaxHeapTree.o Statistics.o Subscriber.o
OUT1	= werhauz
CC	= gcc
FLAGS	= -g -c -Wall


# OBJECTS

CallRecord.o: CallRecord.c CallRecord.h
	$(CC) $(FLAGS) CallRecord.c CallRecord.h

RecordList.o: RecordList.c RecordList.h
	$(CC) $(FLAGS) RecordList.c RecordList.h

HashTable.o: HashTable.c HashTable.h
	$(CC) $(FLAGS) HashTable.c HashTable.h

UserInput.o: UserInput.c UserInput.h
	$(CC) $(FLAGS) UserInput.c UserInput.h

HashFunction.o: HashFunction.c HashFunction.h
	$(CC) $(FLAGS) HashFunction.c HashFunction.h

RecordManagement.o: RecordManagement.c RecordManagement.h
	$(CC) $(FLAGS) RecordManagement.c RecordManagement.h

Global.o: Global.c Global.h
	$(CC) $(FLAGS) Global.c Global.h

MaxHeapTree.o: MaxHeapTree.c MaxHeapTree.h
	$(CC) $(FLAGS) MaxHeapTree.c MaxHeapTree.h

Statistics.o: Statistics.c Statistics.h
	$(CC) $(FLAGS) Statistics.c Statistics.h

Subscriber.o: Subscriber.c Subscriber.h
	$(CC) $(FLAGS) Subscriber.c Subscriber.h

main.o: main.c
	$(CC) $(FLAGS) main.c



werhauz: $(OBJS1)
	$(CC) -g -Wall $(OBJS1) $(LIB) -o $(OUT1)



# clean up
clean:
	rm -f $(OBJS1) $(OUT1)
