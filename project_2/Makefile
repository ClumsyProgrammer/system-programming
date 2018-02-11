
all:  jms_coord jms_console jms_pool hello


OBJS1	= jms_coord.o jms_coord_client.o jms_coord_console_channel.o jms_coord_info.o jms_coord_job.o jms_coord_job_list.o jms_coord_pool.o jms_coord_pool_channel.o jms_coord_pool_list.o jms_coord_signal_handler.o jms_coord_update_struct.o jms_coord_utilities.o
OBJS2	= jms_console.o jms_console_utilities.o
OBJS3	= jms_pool.o jms_pool_job.o jms_pool_job_list.o jms_pool_utilities.o
OBJS4	= hello.o


OUT1	= jms_coord
OUT2	= jms_console
OUT3	= jms_pool
OUT4	= hello

CC	= gcc
FLAGS	= -g -c -Wall



# OBJECTS

hello.o: hello.c
	$(CC) $(FLAGS) hello.c

jms_console.o: jms_console.c
	$(CC) $(FLAGS) jms_console.c 

jms_console_utilities.o: jms_console_utilities.c jms_console_utilities.h
	$(CC) $(FLAGS) jms_console_utilities.c jms_console_utilities.h

jms_coord.o: jms_coord.c
	$(CC) $(FLAGS) jms_coord.c

jms_coord_client.o: jms_coord_client.c jms_coord_client.h
	$(CC) $(FLAGS) jms_coord_client.c jms_coord_client.h

jms_coord_console_channel.o: jms_coord_console_channel.c jms_coord_console_channel.h
	$(CC) $(FLAGS) jms_coord_console_channel.c jms_coord_console_channel.h

jms_coord_info.o: jms_coord_info.c jms_coord_info.h
	$(CC) $(FLAGS) jms_coord_info.c jms_coord_info.h

jms_coord_job.o: jms_coord_job.c jms_coord_job.h
	$(CC) $(FLAGS) jms_coord_job.c jms_coord_job.h

jms_coord_job_list.o: jms_coord_job_list.c jms_coord_job_list.h
	$(CC) $(FLAGS) jms_coord_job_list.c jms_coord_job_list.h

jms_coord_pool.o: jms_coord_pool.c jms_coord_pool.h
	$(CC) $(FLAGS) jms_coord_pool.c jms_coord_pool.h

jms_coord_pool_channel.o: jms_coord_pool_channel.c jms_coord_pool_channel.h
	$(CC) $(FLAGS) jms_coord_pool_channel.c jms_coord_pool_channel.h

jms_coord_pool_list.o: jms_coord_pool_list.c jms_coord_pool_list.h
	$(CC) $(FLAGS) jms_coord_pool_list.c jms_coord_pool_list.h

jms_coord_signal_handler.o: jms_coord_signal_handler.c jms_coord_signal_handler.h
	$(CC) $(FLAGS) jms_coord_signal_handler.c jms_coord_signal_handler.h

jms_coord_update_struct.o: jms_coord_update_struct.c jms_coord_update_struct.h
	$(CC) $(FLAGS) jms_coord_update_struct.c jms_coord_update_struct.h

jms_coord_utilities.o: jms_coord_utilities.c jms_coord_utilities.h
	$(CC) $(FLAGS) jms_coord_utilities.c jms_coord_utilities.h

jms_pool.o: jms_pool.c
	$(CC) $(FLAGS) jms_pool.c

jms_pool_job.o: jms_pool_job.c jms_pool_job.h
	$(CC) $(FLAGS) jms_pool_job.c jms_pool_job.h

jms_pool_job_list.o: jms_pool_job_list.c jms_pool_job_list.h
	$(CC) $(FLAGS) jms_pool_job_list.c jms_pool_job_list.h

jms_pool_utilities.o: jms_pool_utilities.c jms_pool_utilities.h
	$(CC) $(FLAGS) jms_pool_utilities.c jms_pool_utilities.h



jms_coord: $(OBJS1)
	$(CC) -g -Wall $(OBJS1) -o $(OUT1)

jms_console: $(OBJS2)
	$(CC) -g -Wall $(OBJS2) -o $(OUT2)

jms_pool: $(OBJS3)
	$(CC) -g -Wall $(OBJS3) -o $(OUT3)

hello: $(OBJS4)
	$(CC) -g -Wall $(OBJS4) -o $(OUT4)



# clean up
clean:
	rm -f $(OBJS1) $(OUT1)
	rm -f $(OBJS2) $(OUT2)
	rm -f $(OBJS3) $(OUT3)
	rm -f $(OBJS4) $(OUT4)
	rm -rf *.gch




