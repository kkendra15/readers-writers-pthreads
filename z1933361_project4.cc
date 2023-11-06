/**
 * @file z1933361_project4.cc
 * @author Kendra Ferguson
 * @date 2023-11-05
 * TA: Ajay Kuma Reddy Kandula
 * 
 * This program uses semaphores to solve the readers-writers problem
 * with readers-first priority, allowing writers to write to a shared 
 * string, and readers to read that string.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


//global variables:
char sharedstr[] = "All work and no play makes Jack a dull boy.";     //shared string
sem_t rw_sem;       //reader and writer
sem_t cs_sem;       //protects readers' crit section
int readcount = 0;  //current number of readers

/**
 * Checks if arguments are valid non-negative integers
 * 
 * @param args - A command line argument
 * @return true if argument is a valid positive number
 * @return false if argument is not a valid positive number
 */
bool isNum(char args[])
{
    //check for negative symbol
    if (args[0] == '-')
        return false;

    for (int i = 0; args[i] != 0; i++)
    {
        if (!isdigit(args[i]))
            return false;
    }
    return true;
}

/**
 * @param param - contains a pointer to the current thread id
 * 
 *  Logic for the writer threads
 */
void *writer(void *param)
{
    long threadid = (long)param;
    //loop until string is empty
    while (sharedstr[0] != '\0')
    {
        sem_wait(&rw_sem);

        printf("writer %ld is writing ...\n", threadid);

        int size = strlen(sharedstr);
        if (size > 0)
            sharedstr[size - 1] = '\0';
        
        sem_post(&rw_sem);

        //sleep for 1 second
        sleep(1);
    } 

    printf("reader %ld is exiting...\n", threadid);
    pthread_exit(NULL);
}

/**
 * @param param - contains a pointer to the current thread id
 * 
 * Logic for the reader threads
 */
void *reader(void *param)
{
    long threadid = (long)param;

    //enter critical section
    while (sharedstr[0] != '\0')
    {
        //wait(mutex)
        sem_wait(&cs_sem);
        readcount++;
        printf("readcount increments to: %d\n", readcount);

        //if first reader, wait for access
        if (readcount == 1)
        {
            sem_wait(&rw_sem);
        }
        
        //signal(mutex) ---- exit critical section
        sem_post(&cs_sem);

        //do reading ----> print out the shared string
        printf("reader %ld is reading ... content : %s\n", threadid, sharedstr);

        //wait(mutex) ----- enter critical section
        sem_wait(&cs_sem);
        readcount--;

        printf("readcount decrements to: %d\n", readcount);

        //signal writer if it is the last reader
        if (readcount == 0)
            sem_post(&rw_sem);


        //signal(mutex) ------ exit crit sec
        sem_post(&cs_sem);
        sleep(1);

        if (sharedstr[0] == '\0') 
            break;
    }

    printf("writer %ld is exiting...\n", threadid);
    pthread_exit(NULL);
}


//takes 2 command line args - # of readers, # of writers
int main (int argc, char *argv[])
{
    int numreaders, numwriters;
    //check for correct number of arguments
    if (argc != 3)
    {
        printf("Incorrent number of arguments");
        exit(EXIT_FAILURE);
    }
    //check for valid command line arguments:
    if (isNum(argv[1]))
        numreaders = atoi(argv[1]);
    if (isNum(argv[2]))
        numwriters = atoi(argv[2]);

    printf("*** Reader-Writer Problem Simulation ***\n");
    printf("Number of reader threads: %d\n", numreaders);
    printf("Number of writer threads: %d\n", numwriters);

    //innitialize semaphores to 1
    if (sem_init(&rw_sem, 0, 1) == -1) 
    {
        perror("semaphore initialization");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&cs_sem, 0, 1) == -1)
    {
        perror("semaphore initialization");
        exit(EXIT_FAILURE);
    }
   
    //create reader and writer threads
    pthread_t rthreads[numreaders]; //will hold reader thread id's
    pthread_t wthreads[numwriters]; //will hold writer thread ids
   
   //create reader threads
   for (int i = 0; i < numreaders; i++)
   {
       long tid = (long)i;
        if (pthread_create(&rthreads[i], NULL, reader, (void*)tid) != 0) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", i);
            exit(EXIT_FAILURE);
        }
   }
   
    //create writer threads
    for (int i = 0; i < numwriters; i++)
    {
        long tid = (long)i;
        if (pthread_create(&wthreads[i], NULL, writer, (void*)tid) != 0) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    //wait for reader threads to finish
    for (int i = 0; i < numreaders; i++)
    {
        if (pthread_join(rthreads[i], NULL) != 0)
        {
            printf("Error joining reader thread%d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    //wait for writer threads to finish
    for (int i = 0; i < numwriters; i++)
    {
        if (pthread_join(wthreads[i], NULL) != 0)
        {
            printf("Error joining reader thread%d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    //cleanup and exit
    printf("All threads are done.\n");
    sem_destroy(&rw_sem);
    sem_destroy(&cs_sem);
    printf("Resources cleaned up.\n");

    return 0;
}