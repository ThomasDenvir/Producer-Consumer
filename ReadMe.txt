Thomas Denvir
April 02/2023
Producer Consumer Problem written with c++ and compiled with gcc

This document is to explain basic questions and to describe how it functions.

1. What does this program do?
It shows the producer consumer problem in action, which is when a producer generates
data for the consumer to use and how to effectively and efficiently handle this process
without having major issues. Mainly starvation(lack of resources) or deadlock(both operations
are stuck and unable to process any data).

2. How was this problem avoided?
The program written uses semaphores, an integer-like variable that is shared between processes
to allow synchronization of the processes on the cpu. It also uses a motex lock, which is a 
way for the active process to get its chance on the cpu by not allowing any changes to the 
table while the locks are active and unlocking once those processes finish. A simpler explanation?
Think of it as two or more people with poor bladder control trying to use the same bathroom. Without 
door locks or a line queue, things would get very messy very fast.

3. Why can't I run the programs?
This program requires the gcc compiler on Linux/Unix based systems. This program was written
and tested all on a Linux/Unix environment; That being Ubuntu v20.04.2 LTS. Cannot be 100%
certain this runs on windows based systems as it has not yet been tested on any windows systems.

The program must be run through the Unix/Linux terminal using the following commands:
$ gcc producer.cpp -pthread -lrt -o producer
$ gcc consumer.cpp -pthread -lrt -o consumer
$ ./producer & ./consumer &

-------------------------------------------------------------------------------------------------

How does it work?
As stated above, it uses semaphores and motex locks to determine which processes are to be active
at which times. 

On starting the processes, it initializes the table, semaphores, and motex lock.

A shared memory location is created and is used to hold the table data for the processes to access,
and then it proceeds to attach them to each process.

The program then proceeds with the producer operation, as without any data made the consumer cannot 
do anything except wait which would create deadlock.

The producer is allowed access to the cpu and generates 2 random numbers between 0-99 and places them
into the table. It them prints out those values in the command line. This leads to the motex locks 
being disabled and a signal send to the semaphores stating the table is full and needs the data 
inside consumed.

This then allows the consumer process to consume the data provided in the table by activating the motex
lock as it moves itself into the table and begins consuming the data. After this the motex lock disables
and the consuemr process signals the table is now empty which is recieved by the producer process.

The cycle then repeats once more and then stops as the main() functions for both processes begin
deconstructing the table, semaphores, and motex lock.

--------------------------------------------------------------------------------------------------

Known issues:

There is an issue I haven't been able to quite pin down yet when it comes to running the process
more than once. Segmentation faults happen on both processes on the first cycle, however works
correctly on the second. Any input on helping me find and solve this bug would be much appricated.